/*****************************************************************************
 * spu_stt.c : spu module to use speech_to_text (stt) audio filter.
 *****************************************************************************
 * Copyright © 2021 Davide Pietrasanta
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

/*****************************************************************************
 * Preamble
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <errno.h>
#include <string.h>

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_filter.h>
#include <vlc_subpicture.h>

#include <vlc_block.h>
#include <vlc_fs.h>
#include <vlc_strings.h> 

#include "../audio_filter/speech_to_text.h"



/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
static int  CreateFilter ( filter_t * );
static void DestroyFilter( filter_t * );
static subpicture_t *Filter( filter_t *, vlc_tick_t );

static const int pi_color_values[] = {
               0xf0000000, 0x00000000, 0x00808080, 0x00C0C0C0,
               0x00FFFFFF, 0x00800000, 0x00FF0000, 0x00FF00FF, 0x00FFFF00,
               0x00808000, 0x00008000, 0x00008080, 0x0000FF00, 0x00800080,
               0x00000080, 0x000000FF, 0x0000FFFF};
static const char *const ppsz_color_descriptions[] = {
               N_("Default"), N_("Black"),
               N_("Gray"), N_("Silver"), N_("White"), N_("Maroon"), N_("Red"),
               N_("Fuchsia"), N_("Yellow"), N_("Olive"), N_("Green"),
               N_("Teal"), N_("Lime"), N_("Purple"), N_("Navy"), N_("Blue"),
               N_("Aqua") };

/*****************************************************************************
 * filter_sys_t: spu_stt filter descriptor
 *****************************************************************************/

/**
    @brief Struct that implements spu_node.     
    @param start Starting time for the subtitle.
    @param end Ending time for the subtitle.
    @param text To store the text. Actual subtitle.
    Useful for spu_stt module.
    
*/
typedef struct{
    vlc_tick_t start;
    vlc_tick_t end; 
    char* text;
} spu_node ;


/**
    @brief filter_sys_t: Filter descriptor
    @param node a spu_node.
    @param i_pos permit relative positioning (top, bottom, left, right, center).
    @param i_pos_x offsets for the display string in the video window.
    @param i_pos_y offsets for the display string in the video window.
    @param format text format.
    @param p_style font control.
*/
typedef struct
{
    vlc_mutex_t lock;
    std::vector<spu_node>* vector_node; //Maybe 
    int i_pos; 
    int i_pos_x, i_pos_y; 
    char *format;
    text_style_t *p_style; 

} filter_sys_t;


// Description
#define MSG_TEXT N_("Text")
#define MSG_LONGTEXT N_( \
    "SPU_STT text to display. " \
    "(Available format strings: " \
    "%Y = year, %m = month, %d = day, %H = hour, " \
    "%M = minute, %S = second, ...)" )
#define POSX_TEXT N_("X offset")
#define POSX_LONGTEXT N_("X offset, from the left screen edge." )
#define POSY_TEXT N_("Y offset")
#define POSY_LONGTEXT N_("Y offset, down from the top." )
#define START_TEXT N_("Starting time")
#define START_LONGTEXT N_("Number of milliseconds the spu_stt filter start to " \
                            "display the subtitle.")
#define END_TEXT N_("Ending time")
#define END_LONGTEXT N_("Number of milliseconds the spu_stt filter end to " \
                            "display the subtitle.")
#define OPACITY_TEXT N_("Opacity")
#define OPACITY_LONGTEXT N_("Opacity (inverse of transparency) of " \
    "overlayed text. 0 = transparent, 255 = totally opaque." )
#define SIZE_TEXT N_("Font size, pixels")
#define SIZE_LONGTEXT N_("Font size, in pixels. Default is 0 (use default " \
    "font size)." )

#define COLOR_TEXT N_("Color")
#define COLOR_LONGTEXT N_("Color of the text that will be rendered on "\
    "the video. This must be an hexadecimal (like HTML colors). The first two "\
    "chars are for red, then green, then blue. #000000 = black, #FF0000 = red,"\
    " #00FF00 = green, #FFFF00 = yellow (red + green), #FFFFFF = white" )

#define POS_TEXT N_("spu_stt position")
#define POS_LONGTEXT N_( \
  "You can enforce the spu_stt position on the video " \
  "(0=center, 1=left, 2=right, 4=top, 8=bottom, you can " \
  "also use combinations of these values, eg 6 = top-right).")

static const int pi_pos_values[] = { 0, 1, 2, 4, 8, 5, 6, 9, 10 };
static const char *const ppsz_pos_descriptions[] =
     { N_("Center"), N_("Left"), N_("Right"), N_("Top"), N_("Bottom"),
     N_("Top-Left"), N_("Top-Right"), N_("Bottom-Left"), N_("Bottom-Right") };

#define CFG_PREFIX "spu_stt-"

#define SPU_STT_HELP N_("Display auto generate subtitles in the video")

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
vlc_module_begin ()
    set_shortname( N_("SPU_STT" ))
    set_description( N_("SPU Speech To Text Module") )
    set_help(SPU_STT_HELP)
    set_callback_sub_source( CreateFilter, 0 )
    set_category( CAT_VIDEO )
    set_subcategory( SUBCAT_VIDEO_SUBPIC )
    add_string( CFG_PREFIX "SPU_STT", "VLC", MSG_TEXT, MSG_LONGTEXT,
                false )

    set_section( N_("Position"), NULL )
    add_integer( CFG_PREFIX "x", 0, POSX_TEXT, POSX_LONGTEXT, true )
    add_integer( CFG_PREFIX "y", 0, POSY_TEXT, POSY_LONGTEXT, true )
    add_integer( CFG_PREFIX "position", -1, POS_TEXT, POS_LONGTEXT, false )
        change_integer_list( pi_pos_values, ppsz_pos_descriptions )

    set_section( N_("Font"), NULL )
    /* 5 sets the default to top [1] left [4] */
    add_integer_with_range( CFG_PREFIX "opacity", 255, 0, 255,
        OPACITY_TEXT, OPACITY_LONGTEXT, false )
    add_rgb(CFG_PREFIX "color", 0xFFFFFF, COLOR_TEXT, COLOR_LONGTEXT)
        change_integer_list( pi_color_values, ppsz_color_descriptions )
    add_integer( CFG_PREFIX "size", 0, SIZE_TEXT, SIZE_LONGTEXT,
                 false )
        change_integer_range( 0, 4096)
    //?? Misc(?)
vlc_module_end ()

//??
/*
static const char *const ppsz_filter_options[] = {
    "urls", "x", "y", "position", "opacity", "color", "size", "speed", "length",
    "ttl", "images", "title", NULL
};
*/

static const struct vlc_filter_operations filter_ops = {
    .source_sub = Filter, .close = DestroyFilter,
};


/*
We have to convert sub_node to spu_node.

struct sub_node {
    int id; //Ignore it for now
    double starting_time; //convert double to vlc_tick_t
    double ending_time; //convert double to vlc_tick_t
    char* text; 
    bool end_sentence; 
}

sub_node describe each word.
We want spu_node to descrive a phrase!
N.B.
For now we return a simple version because we have to understand how to better treat the data!
*/
spu_node* sub_node_to_spu_node(sub_node* sub){

    spu_node* spu = (spu_node*)malloc(sizeof(spu_node));
    
    spu->start = sub->starting_time;
    spu->end = sub->ending_time;
    spu->text = (char*) malloc ( strlen(sub->text)+1 );
    strcpy( spu->text, sub->text );

    //free(sub) //??
    return spu;
}



/*****************************************************************************
 * CreateFilter: allocates spu_stt video filter
 *****************************************************************************/
static int CreateFilter( filter_t *p_filter )
{
    return VLC_SUCCESS;
}
/*****************************************************************************
 * DestroyFilter: destroy spu_stt video filter
 *****************************************************************************/
static void DestroyFilter( filter_t *p_filter )
{
    /* Delete the spu_stt variables */
    //Free
}

/****************************************************************************
 * Filter: the whole thing
 ****************************************************************************
 * This function outputs subpictures at regular time intervals.
 ****************************************************************************/
static subpicture_t *Filter( filter_t *p_filter, vlc_tick_t date )
{
    //?? Do I need subpicture_t or else?
    filter_sys_t *p_sys = p_filter->p_sys;
    subpicture_t *p_spu = NULL;

    
    return p_spu;
}



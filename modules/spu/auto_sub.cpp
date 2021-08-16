/*****************************************************************************
 * auto_sub.cpp : spu module to use speech_to_text (stt) audio filter.
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

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_filter.h>
#include <vlc_block.h>
#include <vlc_fs.h>
#include <vlc_strings.h>
#include <vlc_subpicture.h>

#include "../audio_filter/speech_to_text.hpp"

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
static int  CreateFilter ( filter_t * );
static void DestroyFilter( filter_t * );
static subpicture_t *Filter( filter_t *, vlc_tick_t );

static int AutoSubCallback( vlc_object_t *p_this, char const *psz_var,
                            vlc_value_t oldval, vlc_value_t newval,
                            void *p_data );
static const unsigned int pi_color_values[] = {
               0xf0000000, 0x00000000, 0x00808080, 0x00C0C0C0,
               0x00FFFFFF, 0x00800000, 0x00FF0000, 0x00FF00FF, 0x00FFFF00,
               0x00808000, 0x00008000, 0x00008080, 0x0000FF00, 0x00800080,
               0x00000080, 0x000000FF, 0x0000FFFF};
static const char *const ppsz_color_descriptions[] = {
               N_("Default"), N_("Black"), N_("Gray"),
               N_("Silver"), N_("White"), N_("Maroon"), N_("Red"),
               N_("Fuchsia"), N_("Yellow"), N_("Olive"), N_("Green"),
               N_("Teal"), N_("Lime"), N_("Purple"), N_("Navy"), N_("Blue"),
               N_("Aqua") };

/*****************************************************************************
 * filter_sys_t: auto_sub filter descriptor
 *****************************************************************************/
typedef struct
{
    vlc_mutex_t lock;

    int i_xoff, i_yoff;  /* offsets for the display string in the video window */
    int i_pos; /* permit relative positioning (top, bottom, left, right, center) */
    vlc_tick_t i_timeout;

    char *format; /**< auto_sub text format */
    char *message; /**< auto_sub plain text */

    text_style_t *p_style; /* font control */

    vlc_tick_t last_time;
    vlc_tick_t i_refresh;
} filter_sys_t;

#define MSG_TEXT N_("Text")
#define MSG_LONGTEXT N_( \
    "auto_sub text to display. " \
    "(Available format strings: " \
    "%Y = year, %m = month, %d = day, %H = hour, " \
    "%M = minute, %S = second, ...)" )
#define POSX_TEXT N_("X offset")
#define POSX_LONGTEXT N_("X offset, from the left screen edge." )
#define POSY_TEXT N_("Y offset")
#define POSY_LONGTEXT N_("Y offset, down from the top." )
#define TIMEOUT_TEXT N_("Timeout")
#define TIMEOUT_LONGTEXT N_("Number of milliseconds the auto_sub must remain " \
                            "displayed. Default value is " \
                            "0 (remains forever).")
#define REFRESH_TEXT N_("Refresh period in ms")
#define REFRESH_LONGTEXT N_("Number of milliseconds between string updates. " \
                            "This is mainly useful when using meta data " \
                            "or time format string sequences.")
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

#define POS_TEXT N_("auto_sub position")
#define POS_LONGTEXT N_( \
  "You can enforce the auto_sub position on the video " \
  "(0=center, 1=left, 2=right, 4=top, 8=bottom, you can " \
  "also use combinations of these values, eg 6 = top-right).")

static const int pi_pos_values[] = { 0, 1, 2, 4, 8, 5, 6, 9, 10 };
static const char *const ppsz_pos_descriptions[] =
     { N_("Center"), N_("Left"), N_("Right"), N_("Top"), N_("Bottom"),
     N_("Top-Left"), N_("Top-Right"), N_("Bottom-Left"), N_("Bottom-Right") };

#define CFG_PREFIX "auto_sub-"

#define AUTO_SUB_HELP N_("Display text above the video")

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
vlc_module_begin ()
    set_shortname( N_("auto_sub" ))
    set_description( N_("auto_sub display") )
    set_help(AUTO_SUB_HELP)
    set_callback_sub_source( CreateFilter, 0 )
    set_category( CAT_VIDEO )
    set_subcategory( SUBCAT_VIDEO_SUBPIC )
    add_string( CFG_PREFIX "sub", "Subtitle VLC", MSG_TEXT, MSG_LONGTEXT,
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

    set_section( N_("Misc"), NULL )
    add_integer( CFG_PREFIX "timeout", 0, TIMEOUT_TEXT, TIMEOUT_LONGTEXT,
                 false )
    add_integer( CFG_PREFIX "refresh", 1000, REFRESH_TEXT,
                 REFRESH_LONGTEXT, false )

    add_shortcut( "time" )
vlc_module_end ()

static const char *const ppsz_filter_options[] = {
    "sub", "x", "y", "position", "color", "size", "timeout", "refresh",
    "opacity",
    NULL
};

static const struct vlc_filter_operations filter_ops = {
    .source_sub = Filter, .close = DestroyFilter,
};

/*****************************************************************************
 * CreateFilter: allocates auto_sub video filter
 *****************************************************************************/
static int CreateFilter( filter_t *p_filter )
{
    /* Allocate structure */
    filter_sys_t *p_sys = static_cast<filter_sys_t *>( malloc( sizeof(filter_sys_t) ) );
    p_filter->p_sys = p_sys;
    
    if( p_sys == NULL )
        return VLC_ENOMEM;

    p_sys->p_style = text_style_Create( STYLE_NO_DEFAULTS );
    if(unlikely(!p_sys->p_style))
    {
        free(p_sys);
        return VLC_ENOMEM;
    }
    vlc_mutex_init( &p_sys->lock );

    config_ChainParse( p_filter, CFG_PREFIX, ppsz_filter_options,
                       p_filter->p_cfg );


#define CREATE_VAR( stor, type, var ) \
    p_sys->stor = var_CreateGet##type##Command( p_filter, var ); \
    var_AddCallback( p_filter, var, AutoSubCallback, p_sys );

    CREATE_VAR( i_xoff, Integer, "auto_sub-x" );
    CREATE_VAR( i_yoff, Integer, "auto_sub-y" );
    p_sys->i_timeout = VLC_TICK_FROM_MS(var_CreateGetIntegerCommand( p_filter,
                                                              "auto_sub-timeout" ));
    var_AddCallback( p_filter, "auto_sub-timeout", AutoSubCallback, p_sys );
    p_sys->i_refresh = VLC_TICK_FROM_MS(var_CreateGetIntegerCommand( p_filter,
                                                              "auto_sub-refresh" ));
    var_AddCallback( p_filter, "auto_sub-refresh", AutoSubCallback, p_sys );
    CREATE_VAR( i_pos, Integer, "auto_sub-position" );
    CREATE_VAR( format, String, "auto_sub-sub" );
    p_sys->message = NULL;
    p_sys->p_style->i_font_alpha = var_CreateGetIntegerCommand( p_filter,
                                                            "auto_sub-opacity" );
    var_AddCallback( p_filter, "auto_sub-opacity", AutoSubCallback, p_sys );
    p_sys->p_style->i_features |= STYLE_HAS_FONT_ALPHA;
    CREATE_VAR( p_style->i_font_color, Integer, "auto_sub-color" );
    p_sys->p_style->i_features |= STYLE_HAS_FONT_COLOR;
    CREATE_VAR( p_style->i_font_size, Integer, "auto_sub-size" );

    /* Misc init */
    p_filter->ops = &filter_ops;
    p_sys->last_time = 0;

    return VLC_SUCCESS;
}
/*****************************************************************************
 * DestroyFilter: destroy auto_sub video filter
 *****************************************************************************/
static void DestroyFilter( filter_t *p_filter )
{
    filter_sys_t *p_sys = static_cast<filter_sys_t *>( p_filter->p_sys );

    /* Delete the auto_sub variables */
#define DEL_VAR(var) \
    var_DelCallback( p_filter, var, AutoSubCallback, p_sys ); \
    var_Destroy( p_filter, var );
    DEL_VAR( "auto_sub-x" );
    DEL_VAR( "auto_sub-y" );
    DEL_VAR( "auto_sub-timeout" );
    DEL_VAR( "auto_sub-refresh" );
    DEL_VAR( "auto_sub-position" );
    DEL_VAR( "auto_sub-sub" );
    DEL_VAR( "auto_sub-opacity" );
    DEL_VAR( "auto_sub-color" );
    DEL_VAR( "auto_sub-size" );

    text_style_Delete( p_sys->p_style );
    free( p_sys->format );
    free( p_sys->message );
    free( p_sys );
}

/****************************************************************************
 * Filter: the whole thing
 ****************************************************************************
 * This function outputs subpictures at regular time intervals.
 ****************************************************************************/
static subpicture_t *Filter( filter_t *p_filter, vlc_tick_t date )
{
    filter_sys_t *p_sys = static_cast<filter_sys_t *>( p_filter->p_sys );
    subpicture_t *p_spu = NULL;
    char *msg;

    vlc_mutex_lock( &p_sys->lock );
    if( p_sys->last_time + p_sys->i_refresh > date )
        goto out;

    msg = vlc_strftime( p_sys->format ? p_sys->format : "" );
    if( unlikely( msg == NULL ) )
        goto out;
    if( p_sys->message != NULL && !strcmp( msg, p_sys->message ) )
    {
        free( msg );
        goto out;
    }
    free( p_sys->message );
    p_sys->message = msg;

    p_spu = filter_NewSubpicture( p_filter );
    if( !p_spu )
        goto out;

    video_format_t vfmt;
    video_format_Init( &vfmt, VLC_CODEC_TEXT );
    vfmt.i_sar_den = vfmt.i_sar_num = 1;
    p_spu->p_region = subpicture_region_New( &vfmt );
    if( !p_spu->p_region )
    {
        subpicture_Delete( p_spu );
        p_spu = NULL;
        goto out;
    }

    p_sys->last_time = date;

    p_spu->p_region->p_text = text_segment_New( msg ); //This set the sub
    p_spu->i_start = date;
    p_spu->i_stop  = p_sys->i_timeout == 0 ? 0 : date + p_sys->i_timeout;
    p_spu->b_ephemer = true;

    /*
    //TEST
    printf("MESSAGE: ");
    for(size_t i=0; i < strlen(msg); i++){
        printf("%c", msg[i]);
    }
    printf("\n");

    char * new_str ;
    new_str = malloc(strlen(msg)*2 + 1);
    new_str[0] = '\0';   // ensures the memory is an empty string
    strcat(new_str,msg);
    strcat(new_str,msg);

    //free(msg);
    msg = new_str;
    //TEST
    */
    

    sub_node* p_sub_node;
    p_sub_node = GetSubNode( p_filter );
    if ( p_sub_node == nullptr || p_sub_node == NULL ){
        std::cout << "auto null" << std::endl;
        p_sub_node = new sub_node();
        char str[] = "auto.cpp";
        p_sub_node->text = (char*)malloc(sizeof(strlen(str)+1));
        strncpy(p_sub_node->text, str, strlen(str));
        p_sub_node->text[strlen(str)] = '\0';
    }
    if( p_sub_node->starting_time == 0 )
    {
        p_sub_node->starting_time = 1;
        std::cout << "AUTO=" <<  p_sub_node << std::endl;
    }


    /*  where to locate the string: */
    if( p_sys->i_pos < 0 )
    {   /*  set to an absolute xy */
        p_spu->p_region->i_align = SUBPICTURE_ALIGN_LEFT | SUBPICTURE_ALIGN_TOP;
        p_spu->b_absolute = true;
    }
    else
    {   /* set to one of the 9 relative locations */
        p_spu->p_region->i_align = p_sys->i_pos;
        p_spu->b_absolute = false;
    }

    p_spu->p_region->i_x = p_sys->i_xoff;
    p_spu->p_region->i_y = p_sys->i_yoff;

    p_spu->p_region->p_text->style = text_style_Duplicate( p_sys->p_style );

out:
    vlc_mutex_unlock( &p_sys->lock );
    return p_spu;
}

/**********************************************************************
 * Callback to update params on the fly
 **********************************************************************/
static int AutoSubCallback( vlc_object_t *p_this, char const *psz_var,
                            vlc_value_t oldval, vlc_value_t newval,
                            void *p_data )
{
    filter_sys_t *p_sys = (filter_sys_t *) p_data;

    VLC_UNUSED(oldval);
    VLC_UNUSED(p_this);

    vlc_mutex_lock( &p_sys->lock );
    if( !strcmp( psz_var, "auto_sub-sub" ) )
    {
        free( p_sys->format );
        p_sys->format = strdup( newval.psz_string );
    }
    else if ( !strcmp( psz_var, "auto_sub-x" ) )
    {
        p_sys->i_xoff = newval.i_int;
    }
    else if ( !strcmp( psz_var, "auto_sub-y" ) )
    {
        p_sys->i_yoff = newval.i_int;
    }
    else if ( !strcmp( psz_var, "auto_sub-color" ) )
    {
        p_sys->p_style->i_font_color = newval.i_int;
    }
    else if ( !strcmp( psz_var, "auto_sub-opacity" ) )
    {
        p_sys->p_style->i_font_alpha = newval.i_int;
    }
    else if ( !strcmp( psz_var, "auto_sub-size" ) )
    {
        p_sys->p_style->i_font_size = newval.i_int;
    }
    else if ( !strcmp( psz_var, "auto_sub-timeout" ) )
    {
        p_sys->i_timeout = VLC_TICK_FROM_MS(newval.i_int);
    }
    else if ( !strcmp( psz_var, "auto_sub-refresh" ) )
    {
        p_sys->i_refresh = VLC_TICK_FROM_MS(newval.i_int);
    }
    else if ( !strcmp( psz_var, "auto_sub-position" ) )
    /* willing to accept a match against auto_sub-pos */
    {
        p_sys->i_pos = newval.i_int;
    }

    free( p_sys->message );
    p_sys->message = NULL; /* force update */

    vlc_mutex_unlock( &p_sys->lock );
    return VLC_SUCCESS;
}

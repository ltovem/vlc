/*****************************************************************************
 * speech_to_text.cpp : Audio Filter to convert speech to text (transcribe).
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
/**
	@file speech_to_text.cpp
	@brief Audio Filter to convert speech to text (transcribe).
*/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

//#include "speech_to_text_vosk.h"
//#include <vosk_api.h>
#include <iostream> //Temporary

#include <queue>

#include <vlc_common.h>
#include <vlc_plugin.h>

#include <vlc_aout.h>
#include <vlc_filter.h>


/*****************************************************************************
 * Local prototypes
 *****************************************************************************/

static int      Open        ( vlc_object_t * );
static void     Close       ( filter_t * );
static block_t *DoWork ( filter_t *, block_t * );

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/

vlc_module_begin ()
    set_shortname (N_("SpeechToText"))
    set_description (N_("Speech To Text filter"))
    set_category (CAT_AUDIO)
    set_subcategory (SUBCAT_AUDIO_AFILTER)
    add_shortcut( "stt" )
    set_capability ("audio filter", 0)
    set_callback( Open )
vlc_module_end ()

/**
    @brief filter_sys_t: Filter descriptor

    @param model Vosk model.
    @param recognizer Vosk recognizer.
    @param text To store the text.
    @param temp Support std::string for text.
    @param time Boolean.
    @param verbose Integer. 
*/
typedef struct
{
    //VoskModel *model;
    //VoskRecognizer *recognizer;
    std::string* text; 
    std::string* temp;
    bool time;
    int verbose;

} filter_sys_t;

//Temporary struct, will be imported
//from speech_to_text_vosk.h
/**
    @brief Struct that implements sub_node. 
    Useful for sub_stt module.
*/
struct sub_node {
    int id;
    double starting_time;
    double ending_time;
    char* text;
    //True if the word marks the end of a sentence.
    bool end_sentence;

    /**
        @brief Default constructor
    */
    sub_node(){
        id = 0;
        starting_time = 0;
        ending_time = 0;
        text = NULL;
        end_sentence = false;
    }

};

/**
    @brief Open: initialize filter

    Aim to convert from .raw to .wav
    Signed 16 bit PCM, Little-endian, 48000Hz, Mono.

    @param p_this A filter_t.
*/
static int Open( vlc_object_t *p_this )
{
    filter_t *p_filter = (filter_t *)p_this;

    filter_sys_t *p_sys = static_cast<filter_sys_t *>( malloc( sizeof(filter_sys_t) ) );
    p_filter->p_sys = p_sys;

    if( unlikely(!p_sys) )
        return VLC_ENOMEM;

    //p_filter setup
    p_filter->fmt_in.audio.i_format = VLC_CODEC_S16L; //To Signed 16 bit
    p_filter->fmt_in.audio.i_rate = 48000; //To 48000 Hz
    p_filter->fmt_in.audio.i_channels = 1; //To mono
    p_filter->fmt_in.audio.i_physical_channels = 1; //To mono

    aout_FormatPrepare(&p_filter->fmt_in.audio);
    p_filter->fmt_out.audio = p_filter->fmt_in.audio;

    //p_sys setup
    p_sys->text = new std::string("c");
    p_sys->temp = new std::string("");
    p_sys->time = false;
    p_sys->verbose = -1;

    //VLC Core will call DoWork function, when it need to filter_audio.
    static const struct vlc_filter_operations filter_ops =
    {
        .filter_audio = DoWork, .close = Close,
    };
    p_filter->ops = &filter_ops;

    msg_Dbg( p_filter, "SpeechToText successfully initialized" );
    return VLC_SUCCESS; //No error. 
}

/**
    @brief DoWork: process samples buffer

    Aim to convert speech to text (transcribe).

    @param p_filter A filter_t.
    @param p_block A block_t.
*/
static block_t *DoWork( filter_t *p_filter, block_t *p_block )
{   
    int nread=  p_block->i_buffer; //size of the buffer
    unsigned char* buf = p_block->p_buffer; //actual data in the buffer

    //std::cout << "SIZE:" << nread << std::endl; //Around 1900-2800

    filter_sys_t *p_sys = static_cast<filter_sys_t *> (p_filter->p_sys);
    *p_sys->text += "C";
    std::cout << "TEXT=" << *p_sys->text << std::endl;
    std::cout << p_sys->time << std::endl;
    std::cout << p_sys->verbose << std::endl;

    msg_Dbg( p_filter, "SpeechToText successfully processed" );
    (void) p_filter;
    return p_block;
}

//?? Possible solution:
//1 - Read all the audio file, store it and than process it using transcribe_vlc.
//2 - Declare *model and *recognizer  and pass it to DoWork as input argoment.
//3 - If performance are lower than before use a Queue to store data and read with chosen buf size.

/**
    //?? Necessary only your filter allocates 
    some resources that need be freed.
    
    @brief Close: close filter

    Close filter and free memory.

    @param p_filter A filter_t.
*/
static void Close( filter_t *p_filter )
{
    filter_sys_t *p_sys = static_cast<filter_sys_t *> (p_filter->p_sys);
    //free( p_sys->model );
    //free( p_sys->recognizer );
    free( p_sys->text );
    free( p_sys->temp );
    free( p_sys );
    msg_Dbg( p_filter, "SpeechToText successfully closed" );
}


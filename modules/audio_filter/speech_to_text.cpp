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
    @brief Open: initialize filter

    Aim to convert from .raw to .wav
    Signed 16 bit PCM, Little-endian, 48000Hz, Mono.

    @param p_this A filter_t.
*/
static int Open( vlc_object_t *p_this )
{
    filter_t *p_filter = (filter_t *)p_this;

    p_filter->fmt_in.audio.i_format = VLC_CODEC_S16L; //To Signed 16 bit
    p_filter->fmt_in.audio.i_rate = 48000; //To 48000 Hz
    p_filter->fmt_in.audio.i_channels = 1; //To mono
    p_filter->fmt_in.audio.i_physical_channels = 1; //To mono

    aout_FormatPrepare(&p_filter->fmt_in.audio);
    p_filter->fmt_out.audio = p_filter->fmt_in.audio;

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

    msg_Dbg( p_filter, "SpeechToText successfully processed" );
    (void) p_filter;
    return p_block;
}

//?? Possible solution:
//1 - Read all the audio file, store it and than process it using transcribe_vlc.
//2 - Declare *model and *recognizer  and pass it to DoWork as input argoment.

/**
    //?? Necessary only your filter allocates 
    some resources that need be freed.
    
    @brief Close: close filter

    Close filter and free memory.

    @param p_filter A filter_t.
*/
static void Close( filter_t *p_filter )
{
    msg_Dbg( p_filter, "SpeechToText successfully closed" );
}


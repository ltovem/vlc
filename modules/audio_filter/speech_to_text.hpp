/*****************************************************************************
 * speech_to_text.h 
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
#ifndef SPEECH_TO_TEXT_HPP
#define SPEECH_TO_TEXT_HPP

#include <iostream> // for operator <<
#include <vlc_common.h> // for GetSubNode()
#include <vlc_plugin.h> // for GetSubNode()

/**
    @brief Struct that implements sub_node.

    Useful for sub_stt module and speech_to_text.cpp.

    @param id Positive number, ID.
    @param staring_time Starting time for the subtitle.
    @param ending_time Ending time for the subtitle.
    @param text To store the text. 
    @param end_sentence True if the word marks the end of a sentence.
*/
typedef struct sub_node {
    int id;
    double starting_time;
    double ending_time;
    char* text;
    bool end_sentence;

    /**
        @brief Default constructor
    */
    sub_node(){
        id = 0;
        starting_time = 0;
        ending_time = 0;
        text = nullptr;
        end_sentence = false;
    }

} sub_node;


/**
    @brief Overload operator <<.

    It allows printing via std::ostream.

	@param out std::ostream
	@param srt sub_node
*/
std::ostream & operator << (std::ostream &out, sub_node &srt) {

	out << "{ \"id\" : ";
    out << srt.id;
    out << "\n";
    out << " \"starting_time\" : ";
    out << srt.starting_time;
    out << "\n";
    out << " \"ending_time\" : ",
    out << srt.ending_time;
    out << "\n";
    out << " \"text\" : ";
    out << srt.text;
    out << "\n";
    out << " \"end_sentence\" : ";
    out << srt.end_sentence;
    out << "}\n";

	return out;
}

/**
    @brief Overload operator <<.

    It allows printing via std::ostream.

	@param out std::ostream
	@param srt sub_node
*/
std::ostream & operator << (std::ostream &out, sub_node *srt) {

	out << "{ \"id\" : ";
    out << srt->id;
    out << "\n";
    out << " \"starting_time\" : ";
    out << srt->starting_time;
    out << "\n";
    out << " \"ending_time\" : ",
    out << srt->ending_time;
    out << "\n";
    out << " \"text\" : ";
    out << srt->text;
    out << "\n";
    out << " \"end_sentence\" : ";
    out << srt->end_sentence;
    out << "}\n";

	return out;
}

/**
    @brief It allows passing the variable through address.

	@param p_object A vlc_object_t.
    @return Address of the sub_node variable.
*/
static sub_node *GetSubNode( vlc_object_t *p_object )
{
    return static_cast<sub_node *>( var_GetAddress(VLC_OBJECT(vlc_object_instance(p_object)),
                          "stt-struct") );
}
#define GetSubNode(a) static_cast<sub_node *>( GetSubNode( VLC_OBJECT(a) ) )

#endif //SPEECH_TO_TEXT_HPP
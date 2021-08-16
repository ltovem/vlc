#ifndef SPEECH_TO_TEXT_VOSK_HPP
#define SPEECH_TO_TEXT_VOSK_HPP

#include <vosk_api.h>
#include "speech_to_text.h"
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <iostream>
#include <iterator>
#include <fstream>


/**
	@file Name.h
	@brief Transcriber file.

    TO DO 
    -Use VLC to conver to .wav (than delete ffmpeg)
*/

/**
    @brief Struct that implements srt_node. 
    Useful for .srt files.

    @param id Positive number, ID.
    @param staring_time Starting time for the subtitle.
    @param ending_time Ending time for the subtitle.
    @param text To store the text. 
    @param end_sentence True if the word marks the end of a sentence.
*/
struct srt_node {
    int id;
    std::string starting_time;
    std::string ending_time;
    std::string text;
    bool end_sentence;

    /**
        @brief Default constructor
    */
    srt_node(){
        id = 0;
        starting_time = "";
        ending_time = "";
        text = "";
        end_sentence = false;
    }

    /**
        @brief Constructor that instantiates a deep copy of v
        @param v srt_node to copy
    */
    srt_node(srt_node* v){
        id = v->id;
        starting_time = v->starting_time;
        ending_time = v->ending_time;
        text = v->text;
        end_sentence = v->end_sentence;
    }

};

/**
    @brief Overload operator <<.

    It allows printing via std::ostream.

	@param out std::ostream
	@param srt srt_node
*/
std::ostream & operator << (std::ostream &out, srt_node& srt) {

	out << "{ \"id\" : ";
    out << srt.id;
    out << "\n";
    out << " \"starting_time\" : "+ srt.starting_time +"\n";
    out << " \"ending_time\" : "+ srt.ending_time +"\n";
    out << " \"text\" : " + srt.text + "\n";
    out << " \"end_sentence\" : ";
    out << srt.end_sentence;
    out << "}\n";

	return out;
}


/**
    @brief Append but adjusted for transcribe_vosk outputs.
    @param str1  First string.
    @param str2  Second string.
    @param time  Default false, time should have the same value of time in transcribe_vosk function.
    @return  Pointer to another string.
*/
std::string  append_transcribe_vosk(const std::string str1, const std::string str2, bool time = false){

    //We first need to check if str1 and str2 are empty
    if( str1.empty() && str2.empty() ){
        std::string copy("");
        return copy;
    }

    if( str1.empty() ){
        std::string copy(str2);
        return copy;
    } 

    if( str2.empty() ){
        std::string copy(str1);
        return copy;
    }    
    
    if ( !time )  {
        //JSON without time
        std::string string1(str1);
        std::string string2(str2);

        //Delete last 2 char from string1;
        string1.resize(string1.size() - 2);
        
        size_t index = string2.find("text");

        std::string string2_cut = string2.substr(index+9, string2.size());
        string1.append(" ");
        string1.append(string2_cut);
        
        return string1;
    }
    else{
        //JSON with time
        std::string string1(str1);
        std::string string2(str2);

        //Delete last 2 char from string1;
        string1.resize(string1.size() - 2);
        
        size_t index1 = string1.find("text");
        size_t index2_start = string2.find("result");
        size_t index2_end = string2.find("text");

        std::string string1_cut = string1.substr(0, index1-6);
        string1_cut.insert(string1_cut.length()-7, ".");
        std::string string2_cut = string2.substr(index2_start+11, index2_end-index2_start-16);
        string1_cut.append(", ");
        string1_cut.append(string2_cut);
        string1_cut.append("}");

        return string1_cut;
    }

    return std::string("");
}

/**
    @brief Replace all oldW in s with newW.
    @param s  Original string.
    @param oldW  Old word.
    @param newW  New word.
    @return  Pointer to another string with all occurrence of oldW replaced with newW.
*/
std::string replaceWord(const std::string s, const std::string oldW, const std::string newW){
    
    size_t index = 0;
    std::string str(s);
    while (true) {
        /* Locate the substring to replace. */
        index = str.find(oldW, index);
        if (index == std::string::npos) break;

        /* Make the replacement. */
        str.replace(index, newW.length(), newW);

        /* Advance index forward so the next iteration doesn't pick it up as well. */
        index += newW.length();
    }

    return str;
}


/**
    @brief Count occurrence of pattern in str. 
        Not meant to chatch multiple occurrence in the same characters.
    @param str  Original string.
    @param pattern Pattern to find.
    @return  Number of time pattern occurs in str.
*/
size_t number_of_occurrence(const std::string str, const std::string pattern){
    size_t occurrences = 0;
    std::string::size_type pos = 0;
    size_t lenght_pattern = pattern.length();

    while ((pos = str.find(pattern, pos )) != std::string::npos) {
            occurrences++;
            pos += lenght_pattern;
    }
    return occurrences;
}


/**
    @brief Write the input string into a file.
    @param str String you want to write in file.
    @param path File path.
    @return  Return true if the process ended without errors.
*/
bool write_in_file(std::string str, const std::string path){
    std::ofstream stream;

    stream.open(path);
    
    if( !stream ){
        // Opening file failed
        return false;
    }
    
    stream << str << std::endl;
    return true;
}

/**
 *  Temporary
    @brief Convert audio into .WAV (PCM S16le mono) using ffmpeg.
    @param input_file Input audio file to convert.
    @return  Return the name of the output .wav file.
*/
const std::string ffmpeg_converter(const std::string input_file){
    std::string  s1 = "ffmpeg -hide_banner -loglevel error -i "; //set ffmpeg verbosity
    std::string  s2 = " -acodec pcm_s16le -ac 1 -ar 16000 ";

    char sep = '.';
    size_t  index= input_file.find(sep);

    std::string  output_file(input_file);
    output_file = output_file.substr(0, index);

    std::string  wav = "_PCM_mono.wav";
    output_file += wav;

    std::string  s3 = " -y";

    std::string command_line = s1 + input_file + s2 + output_file + s3;
    system(command_line.c_str());

    return output_file.c_str();
    //system("ffmpeg -hide_banner -loglevel error -i input_file -acodec pcm_s16le -ac 1 -ar 16000 output_file.wav");
}

/**
    @brief Transform string time to string in clock time.
        Transform this "1.240" into "00:00:01,240".
    @param time_str Time, in s and ms.
    @return  String in clock time, max precision of 3 decimal of second.
*/
std::string to_hour_min_sec_ms(std::string time_str){

    size_t index = time_str.find(".");
    std::string not_decimal = time_str.substr(0, index);
    std::string decimal;

    if( index == std::string::npos )
        decimal = "000";
    else
        decimal = time_str.substr(index+1, 3);

    int input_seconds = stoi(not_decimal);

    size_t hours = (input_seconds / 60 / 60) % 24;
    size_t minutes = (input_seconds / 60) % 60;
    size_t seconds = input_seconds % 60;

    std::string hours_str = std::to_string(hours);
    std::string minutes_str = std::to_string(minutes);
    std::string seconds_str = std::to_string(seconds);

    if( hours_str.length() == 1 )
        hours_str = "0" + hours_str;

    if( minutes_str.length() == 1 )
        minutes_str = "0" + minutes_str;

    if( seconds_str.length() == 1 )
        seconds_str = "0" + seconds_str; 


    return hours_str + ":" + minutes_str + ":" + seconds_str + "," + decimal;
}


/**
    @brief Transform json string into vector<srt_node>.
    @param json A json string. Should be the output of transcribe_vosk.
    @return  A vector<srt_node>.
*/
std::vector<srt_node> json_to_srt_node(const std::string json){
    char pattern [] = "\"word\"";
    size_t n_word = number_of_occurrence(json, pattern);
    std::vector<srt_node> srt_vector;
    std::vector<srt_node>::iterator it;
    size_t index_end, index_start, index_word;
    size_t index_sep;
    size_t counter, gap;
    std::string str(json);

    it = srt_vector.begin();

    srt_node temp_srt_node;

    for(counter = 0; counter < n_word; counter++){
        temp_srt_node.id = counter+1;

        index_end = str.find("\"end\"");
        index_sep = str.find(",", index_end);
        index_start = str.find("\"start\"", index_sep);
        
        //end_time
        gap = index_sep - index_end;
        temp_srt_node.ending_time = str.substr(index_end, gap);
        temp_srt_node.ending_time = temp_srt_node.ending_time.substr(8, gap);
        temp_srt_node.ending_time = to_hour_min_sec_ms(temp_srt_node.ending_time);
        

        index_sep = str.find(",", index_start);
        index_word = str.find("\"word\"", index_start);
        //start_time
        gap = index_sep - index_start;
        temp_srt_node.starting_time = str.substr(index_start, gap);
        temp_srt_node.starting_time = temp_srt_node.starting_time.substr(10, gap);
        temp_srt_node.starting_time = to_hour_min_sec_ms(temp_srt_node.starting_time);


        index_sep = str.find("}", index_word);
        //text
        gap = index_sep - index_word - 3;
        temp_srt_node.text = str.substr(index_word, gap);
        temp_srt_node.text = temp_srt_node.text.substr(9, gap-11);

        //end_sentence
        temp_srt_node.end_sentence = ( temp_srt_node.text.find(".") != std::string::npos );

        //std::cout << temp_srt_node << std::endl;

        srt_vector.insert( srt_vector.end() , srt_node(temp_srt_node) );

        //cut string
        str = str.substr(index_sep, str.length());
    }

    return srt_vector;

} 

/**
    @brief Transform json string into vector<sub_node>.
    @param json A json string. Should be the output of transcribe_vosk.
    @return  A vector<sub_node>.
*/
std::vector<sub_node> json_to_sub_node(const std::string json){
    char pattern [] = "\"word\"";
    size_t n_word = number_of_occurrence(json, pattern);
    std::vector<sub_node> sub_vector;
    std::vector<sub_node>::iterator it;
    size_t index_end, index_start, index_word;
    size_t index_sep;
    size_t counter, gap;
    std::string str(json);

    std::string temp_text;
    std::string temp_ending_time;
    std::string temp_starting_time;
    

    it = sub_vector.begin();

    sub_node temp_sub_node;

    for(counter = 0; counter < n_word; counter++){
        temp_sub_node.id = counter+1;

        index_end = str.find("\"end\"");
        index_sep = str.find(",", index_end);
        index_start = str.find("\"start\"", index_sep);
        
        //end_time
        gap = index_sep - index_end;
        temp_ending_time = str.substr(index_end, gap);
        temp_ending_time = temp_ending_time.substr(8, gap);
        temp_sub_node.ending_time = std::stod(temp_ending_time); //Convert the string to time
        

        index_sep = str.find(",", index_start);
        index_word = str.find("\"word\"", index_start);
        //start_time
        gap = index_sep - index_start;
        temp_starting_time = str.substr(index_start, gap);
        temp_starting_time = temp_starting_time.substr(10, gap);
        temp_sub_node.starting_time = std::stod(temp_starting_time); //Convert the string to time


        index_sep = str.find("}", index_word);
        //text
        gap = index_sep - index_word - 3;
        temp_text = str.substr(index_word, gap);
        temp_text = temp_text.substr(9, gap-11);
        temp_sub_node.text = (char*) malloc ( sizeof(char) * (temp_text.length()+1) );
        strcpy(temp_sub_node.text, temp_text.c_str()); //Remember to free them

        //end_sentence
        temp_sub_node.end_sentence = ( temp_text.find(".") != std::string::npos );

        //std::cout << temp_sub_node << std::endl;

        sub_vector.insert( sub_vector.end() , sub_node(temp_sub_node) );

        //cut string
        str = str.substr(index_sep, str.length());
    }

    return sub_vector;

} 

/**
    @brief Transform json string into string for the .srt file.
    @param json A json string. Should be the output of transcribe_vosk.
    @return  A string for the .srt file.
*/
std::string json_to_srt(std::string json){

    std::vector<srt_node> srt_vector = json_to_srt_node(json);
    
    bool new_phrase = true;
    std::string str("");
    std::string temp_text, temp_header;
    size_t i;
    size_t id = 1;

    for(i=0; i < srt_vector.size(); i++){

        if( new_phrase ){
            temp_header = std::to_string(id) + "\n";
            id ++;
            temp_header += srt_vector[i].starting_time + " --> ";

            temp_text = srt_vector[i].text.substr(1, srt_vector[i].text.length()-1);
            new_phrase = false;
        }
        else{
            temp_text = temp_text.substr(0, temp_text.length()-1); //delete "
            temp_text += " " + srt_vector[i].text.substr(1, srt_vector[i].text.length()-1);
        }

        if( srt_vector[i].end_sentence ){
            temp_header += srt_vector[i].ending_time + "\n";
            str += temp_header + temp_text.substr(0, temp_text.length() -1 ) + "\n\n";
            new_phrase = true;

            temp_header = "";
            temp_text = "";
        }
    }

    if( temp_header.compare("") != 0 ){
        //The text is ended but not with a dot (.)
        temp_header += srt_vector[i-1].ending_time + "\n";
        str += temp_header + temp_text.substr(0, temp_text.length() -1 ) + "\n\n";
    }

    return str;
}


/**
    @brief Transcribe .wav file using Vosk library.
    @param file_name Path of the file to transcribe.
    @param time Default false, If true, show the time related to the words.
    @param verbose Level of verbosity.
 *     -0 : default value to print info and error messages but no debug
 *     -less than 0 : don't print info messages
 *     -greather than 0 : more verbose mode
    @return  
*/
std::string transcribe_vosk(const std::string file_name , bool time = false, int verbose = -1){

    //verbosity level
    vosk_set_log_level(verbose);
    if( verbose >= 0)
        std::cout<<"Name audio file: "<< file_name <<std::endl;

    FILE *wavin;
    char buf[3200];
    int nread, final;
    std::string text = "";
    char unk [] = "[unk]";
    char rep [] = "[???]";
    std::string temp = "";

    VoskModel *model = vosk_model_new("vosk-api-master/model");
    VoskRecognizer *recognizer = vosk_recognizer_new(model, 16000.0);

    if( time ){
        vosk_recognizer_set_words(recognizer, 1);
    }

    wavin = fopen(file_name.c_str(), "rb");
    fseek(wavin, 44, SEEK_SET); //Skip the header since read only .WAV
    
    std::cout << "Working..." << std::endl;

    while (!feof(wavin)) {
        nread = fread(buf, 1, sizeof(buf), wavin);
        final = vosk_recognizer_accept_waveform(recognizer, buf, nread);
        if (final) {
            temp = replaceWord(std::string(vosk_recognizer_result(recognizer)), unk, rep);
            text = append_transcribe_vosk(text, std::string(temp), time);

            //std::cout << "TEXT:" << text << std::endl;
            //std::cout << "TEMP:" << temp << std::endl;

            if( verbose >= 0)
                std::cout<< text <<std::endl;

        } else {
            //if you want also partial phrase.
            if( verbose >= 0)
                std::cout<< vosk_recognizer_partial_result(recognizer) <<std::endl;
        }
    }

    temp = replaceWord(std::string(vosk_recognizer_result(recognizer)), unk, rep);
    text = append_transcribe_vosk(text, std::string(temp), time);

    if( verbose >= 0)
        std::cout<<"Final result: \n"<< text <<std::endl;

    vosk_recognizer_free(recognizer);
    vosk_model_free(model);
    fclose(wavin);
    return text;
}


#endif //SPEECH_TO_TEXT_VOSK_HPP
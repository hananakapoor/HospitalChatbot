#ifndef __BASIC_SERVICES_H__
#define __BASIC_SERVICES_H__

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <map>
#include "speech_services.h"

#define AUDIO_FOLDER "voice_men/"
#define TEMP_FOLDER "temp/"
#define SILENCE_FOLDER "silence/"

class basic_services {
private:
    struct service_agent {
        unsigned int word_rate;
    };

    unsigned int last_agent_id;
    unsigned int max_number_of_agents;
    unsigned int sample_rate;
    
    speech_services speech;

public:
    void clear_all_agents() {
        last_agent_id = 0;
    }

    basic_services(unsigned int _max_number_of_agents) : max_number_of_agents(_max_number_of_agents) {
        clear_all_agents();
        speech.ensure_silence_files(); // Ensure silence files are created on initialization
    }

    ~basic_services() {}

    unsigned int init_an_agent() {
        last_agent_id = (last_agent_id + 1) % max_number_of_agents;
        return last_agent_id;
    }
    std::string to_text(unsigned int service_id, const std::string& file_in) {
        std::string command = "python3 classify_audio.py \"" + file_in + "\"";
        std::cout << "Executing command: " << command << std::endl;

        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) {
            std::cerr << "Failed to run command!" << std::endl;
            return "";
        }

        char buffer[256];
        std::string result;
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }

        pclose(pipe);
        if (!result.empty() && result.back() == '\n') {
            result.pop_back();
        }

        std::cout << "   command: " << result  ;
        return result;
    }

  /*  std::string to_text(unsigned int service_id, const std::string& file_in) {
        std::string command = "python3 stt.py \"" + file_in + "\"";
        std::cout << "Executing command: " << command << std::endl;

        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) {
            std::cerr << "Failed to run command!" << std::endl;
            return "";
        }

        char buffer[256];
        std::string result;
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }

        pclose(pipe);
        if (!result.empty() && result.back() == '\n') {
            result.pop_back();
        }

        return result;
    } */


 
    
    // std::string translate_to_malayalam(const std::string& text) {
    //     std::string command = "python3 tts_mal.py \"" + text + "\"";
    //     char buffer[256];
    //     std::string translated_text = "";
    
    //     FILE* pipe = popen(command.c_str(), "r");
    //     if (!pipe) return "ERROR";
    
    //     while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
    //         translated_text += buffer;
    //     }
    //     pclose(pipe);
    
    //     return translated_text;
    // }
    
    // void to_voice(const std::string &text, const std::string &output_file = "output.wav") {
    //     // std::string command = "espeak -v ml \"" + text + "\" --stdout > " + output_file;
    //     // system(command.c_str());
    //     std::string malayalam_text = translate_to_malayalam(text);
    // std::string command = "espeak -v ml \"" + malayalam_text + "\" --stdout > " + output_file;
    // system(command.c_str());
    // }
    
    
   
    // std::string to_voice(const std::string &text, const std::string &output_file = "output.mp3") {
    //     std::string malayalam_text = translate_to_malayalam(text);
    
    //     if (!malayalam_text.empty()) {
    //         std::cout << "Translated Text: " << malayalam_text << std::endl;
    //         std::string command = "python3 tts.py \"" + malayalam_text + "\" " + output_file;
    //         system(command.c_str());  // Generate speech
    //     }
    
    //     return malayalam_text;  // Return the Malayalam translation
    // }
    
    
    void to_voice(const std::string &text, const std::string &output_file = "output.wav") {
        std::string escapedText;
        for (char c : text) {
            if (c == '"' || c == '\\') {
                escapedText += '\\';
            }
            escapedText += c;
        }

        std::string command = "flite -voice slt -t \"" + escapedText + "\" -o " + output_file;
       
        int result = system(command.c_str());
        if (result != 0) {
            std::cerr << "Flite command execution failed" << std::endl;
        } else {
            std::cout << "Audio saved to " << output_file << std::endl;
        }
    }

    void to_voice_folder(const std::string& sentence, const std::string& output_file) {
        std::string cleaned_sentence = speech.remove_punctuation(sentence);
        cleaned_sentence = speech.to_lowercase(cleaned_sentence);
    
        std::vector<std::string> words = speech.split_sentence(cleaned_sentence);
        std::vector<std::string> paused_files;
        
        std::string temp_folder(TEMP_FOLDER);
        system(("mkdir -p " + temp_folder).c_str());
        std::string paused_folder = temp_folder + "paused/";
        system(("mkdir -p " + paused_folder).c_str());
        
        std::cout << "Adding natural pauses between words..." << std::endl;
        for (size_t i = 0; i < words.size(); i++) {
            std::string input_file = std::string(AUDIO_FOLDER) + words[i] + ".wav";
            std::string paused_file = paused_folder + "word_" + std::to_string(i) + "_paused.wav";
            
            if (i < words.size() - 1) {
                if (speech.add_natural_pause(input_file, paused_file, 0.02f)) {
                    paused_files.push_back(paused_file);
                } else {
                    std::cerr << "Failed to add pause to " << input_file << std::endl;
                    return;
                }
            } else {
                std::string command = "cp " + input_file + " " + paused_file;
                system(command.c_str());
                paused_files.push_back(paused_file);
            }
        }
        
        std::cout << "Combining files with crossfade..." << std::endl;
        std::string combined_output = "combined_audio.wav"; // Temporary combined file
        if (speech.combine_audio_files(paused_files, combined_output, 0.02f)) {
            std::cout << "Adjusting speed and applying light normalization..." << std::endl;
            // Use the output_file parameter for the final output
            std::string command = "sox " + combined_output + " " + output_file + " speed 1.0 norm -0.5 compand 0.01,0.2 -60,-30,-20,-10,-10,-8,0,-4 0 -80";
            system(command.c_str());
            std::cout << "Final output saved as " << output_file << std::endl;
            
            // Clean up temporary files
            system(("rm -rf " + temp_folder).c_str());
            system(("rm " + combined_output).c_str()); // Remove the temporary combined file
        } else {
            system(("rm -rf " + temp_folder).c_str());
        }
    }

    void convert_webm_to_wav(const std::string &input_file, const std::string &output_file) {
        std::string command = "gst-launch-1.0 filesrc location=" + input_file +
                              " ! decodebin ! audioconvert ! audioresample ! wavenc ! filesink location=" + output_file;
        int ret = system(command.c_str());

        if (ret == 0) {
            std::cout << "Conversion successful: " << input_file << " -> " << output_file << std::endl;
        } else {
            std::cerr << "Error converting file." << std::endl;
            exit(1);
        }
    }
};

#endif

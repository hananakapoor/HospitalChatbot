// #ifndef __SPEECH_SERVICES_H__
// #define __SPEECH_SERVICES_H__

// #include <iostream>
// #include <vector>
// #include <string>
// #include <algorithm>
// #include <cctype>
// #include <cstdio>
// #include <cstdlib>
// #include <fstream>
// #include <sstream>
// #include <map>

// #define AUDIO_FOLDER "voice_men/"
// #define TEMP_FOLDER "temp/"
// #define SILENCE_FOLDER "silence/"

// // Helper class containing subfunctions for speech processing
// class speech_services {
// private:
//     int sample_rate;

// public:
//     // Constructor
//     speech_services() : sample_rate(44100) {}

//     // Function to split a sentence into words while preserving punctuation
//     static std::vector<std::pair<std::string, char>> split_sentence_with_punctuation(const std::string& sentence) {
//         std::vector<std::pair<std::string, char>> word_punct_pairs;
//         std::string current_word;
        
//         for (size_t i = 0; i < sentence.length(); i++) {
//             char c = sentence[i];
            
//             if (std::isalnum(c) || c == '\'') {
//                 current_word += c;
//             } else if (std::isspace(c) || c == ',' || c == ';' || c == '.' || c == ':' || c == '?' || c == '"') {
//                 // If we have a word, add it with the punctuation
//                 if (!current_word.empty()) {
//                     char punct = (std::isspace(c)) ? ' ' : c;
//                     word_punct_pairs.push_back({current_word, punct});
//                     current_word.clear();
//                 }
//                 // Handle consecutive punctuation marks
//                 else if (!word_punct_pairs.empty() && !std::isspace(c)) {
//                     word_punct_pairs.back().second = c;
//                 }
//             }
//         }
        
//         // Add the last word if it exists
//         if (!current_word.empty()) {
//             word_punct_pairs.push_back({current_word, ' '});
//         }
        
//         return word_punct_pairs;
//     }

//     // Function to convert a string to lowercase
//     static std::string to_lowercase(const std::string& str) {
//         std::string lower_str = str;
//         std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower);
//         return lower_str;
//     }

//     // Function to escape special characters in a string
//     static std::string escape_text(const std::string& text) {
//         std::string escapedText;
//         for (char c : text) {
//             if (c == '"' || c == '\\') {
//                 escapedText += '\\';
//             }
//             escapedText += c;
//         }
//         return escapedText;
//     }

//     // Function to detect the sample rate of an audio file
//     static int detect_sample_rate(const std::string& audio_file) {
//         std::string command = "soxi -r \"" + audio_file + "\"";
//         FILE* pipe = popen(command.c_str(), "r");
//         if (!pipe) {
//             std::cerr << "Failed to run soxi command!" << std::endl;
//             return 44100; // Default to 44100 Hz if detection fails
//         }

//         char buffer[256];
//         std::string result;
//         while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
//             result += buffer;
//         }
//         pclose(pipe);

//         // Remove any trailing whitespace
//         result.erase(result.find_last_not_of(" \n\r\t") + 1);

//         try {
//             return std::stoi(result);
//         } catch (const std::exception& e) {
//             std::cerr << "Failed to parse sample rate: " << e.what() << std::endl;
//             return 44100; // Default sample rate if parsing fails
//         }
//     }

//     // Function to find a reference audio file to get the sample rate
//     static std::string find_reference_audio_file() {
//         std::string audio_folder(AUDIO_FOLDER);
//         std::string find_command = "find " + audio_folder + " -name \"*.wav\" -type f | head -n 1";
        
//         FILE* pipe = popen(find_command.c_str(), "r");
//         if (!pipe) {
//             std::cerr << "Failed to run find command!" << std::endl;
//             return "";
//         }
        
//         char buffer[256];
//         std::string result;
//         if (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
//             result = buffer;
//             // Remove trailing newline
//             if (!result.empty() && result.back() == '\n') {
//                 result.pop_back();
//             }
//         }
//         pclose(pipe);
        
//         return result;
//     }

//     // Function to get the silence file path for a given punctuation
//     static std::string get_silence_file(char punctuation) {
//         std::string silence_dir(SILENCE_FOLDER);
        
//         switch (punctuation) {
//             case ' ': return silence_dir + "silence_1.wav"; // Space: 1 unit delay
//             case ',': return silence_dir + "silence_2.wav"; // Comma: 2 unit delay
//             case ';': return silence_dir + "silence_3.wav"; // Semicolon: 3 unit delay
//             case '.': return silence_dir + "silence_4.wav"; // Period: 4 unit delay
//             case '?': return silence_dir + "silence_4.wav"; // Question mark: 4 unit delay
//             case ':': return silence_dir + "silence_3.wav"; // Colon: 3 unit delay
//             case '"': return silence_dir + "silence_2.wav"; // Quotation mark: 2 unit delay
//             default: return silence_dir + "silence_1.wav";  // Default: 1 unit delay
//         }
//     }

//     // Function to combine multiple audio files
//     static bool combine_audio_files(const std::vector<std::string>& input_files, const std::string& output_path, float crossfade_time = 0.03f) {
//         if (input_files.empty()) {
//             std::cerr << "No input files provided!" << std::endl;
//             return false;
//         }

//         if (input_files.size() == 1) {
//             std::string command = "cp \"" + input_files[0] + "\" \"" + output_path + "\"";
//             system(command.c_str());
//             return true;
//         }

//         // Use sox to combine all files at once
//         std::string command = "sox";
//         for (const auto& file : input_files) {
//             command += " \"" + file + "\"";
//         }
//         command += " \"" + output_path + "\"";
        
//         int result = system(command.c_str());
//         if (result != 0) {
//             std::cerr << "Error combining audio files. Command: " << command << std::endl;
//             return false;
//         }

//         std::cout << "Audio files successfully combined into " << output_path << std::endl;
//         return true;
//     }

//     // Function to ensure silence files exist with the correct sample rate
//     static void ensure_silence_files(int& sample_rate) {
//         std::string reference_file = find_reference_audio_file();
        
//         if (reference_file.empty()) {
//             std::cout << "No reference audio file found. Using default sample rate of 44100 Hz." << std::endl;
//             sample_rate = 44100;
//         } else {
//             sample_rate = detect_sample_rate(reference_file);
//             std::cout << "Using reference file: " << reference_file << std::endl;
//             std::cout << "Detected sample rate: " << sample_rate << " Hz" << std::endl;
//         }

//         std::string silence_dir(SILENCE_FOLDER);
//         system(("mkdir -p " + silence_dir).c_str());
        
//         // Create silence files of different durations with the matching sample rate
//         std::map<int, std::string> silence_files = {
//             {1, silence_dir + "silence_1.wav"},
//             {2, silence_dir + "silence_2.wav"},
//             {3, silence_dir + "silence_3.wav"},
//             {4, silence_dir + "silence_4.wav"}
//         };
        
//         for (const auto& pair : silence_files) {
//             std::ifstream file(pair.second);
//             float duration = 0.1f * pair.first;  // 0.1s per unit
            
//             // Always recreate the silence files to ensure they have the correct sample rate
//             std::string command = "sox -n -r " + std::to_string(sample_rate) + " " + pair.second + " trim 0.0 " + std::to_string(duration);
//             system(command.c_str());
//             std::cout << "Created silence file: " + pair.second + " with sample rate " + std::to_string(sample_rate) + " Hz" << std::endl;
//         }
//     }

//     // Convert WebM to WAV format
//     static bool convert_webm_to_wav(const std::string& webm_file, const std::string& wav_file) {
//         std::string command = "ffmpeg -i \"" + webm_file + "\" -y \"" + wav_file + "\" 2>/dev/null";
//         int result = system(command.c_str());
//         if (result != 0) {
//             std::cerr << "Error converting WebM to WAV: " << webm_file << " to " << wav_file << std::endl;
//             return false;
//         }
//         std::cout << "Successfully converted " << webm_file << " to " << wav_file << std::endl;
//         return true;
//     }
// };

// #endif // __SPEECH_SERVICES_H__

#ifndef __SPEECH_SERVICES_H__
#define __SPEECH_SERVICES_H__

#include <iostream>
#include <vector>
#include <sndfile.h>
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

#define AUDIO_FOLDER "voice_men/"
#define TEMP_FOLDER "temp/"
#define SILENCE_FOLDER "silence/"

class speech_services {
private:
    unsigned int sample_rate;

public:
    speech_services() : sample_rate(44100) {}

    bool crossfade_files(const std::string& file1, const std::string& file2, const std::string& output_file, float fade_time = 0.03f) {
        SNDFILE *infile1 = nullptr, *infile2 = nullptr, *outfile = nullptr;
        SF_INFO sfinfo1, sfinfo2, sfinfo_out;
        
        memset(&sfinfo1, 0, sizeof(sfinfo1));
        memset(&sfinfo2, 0, sizeof(sfinfo2));
        memset(&sfinfo_out, 0, sizeof(sfinfo_out));
        
        if (!(infile1 = sf_open(file1.c_str(), SFM_READ, &sfinfo1))) {
            std::cerr << "Error opening " << file1 << ": " << sf_strerror(nullptr) << std::endl;
            return false;
        }
        
        if (!(infile2 = sf_open(file2.c_str(), SFM_READ, &sfinfo2))) {
            std::cerr << "Error opening " << file2 << ": " << sf_strerror(nullptr) << std::endl;
            sf_close(infile1);
            return false;
        }
        
        if (sfinfo1.samplerate != sfinfo2.samplerate || sfinfo1.channels != sfinfo2.channels) {
            std::cerr << "Files have different formats" << std::endl;
            sf_close(infile1);
            sf_close(infile2);
            return false;
        }
        
        int fade_frames = static_cast<int>(fade_time * sfinfo1.samplerate);
        
        if (sfinfo1.frames < fade_frames || sfinfo2.frames < fade_frames) {
            std::cerr << "Files too short for crossfade" << std::endl;
            sf_close(infile1);
            sf_close(infile2);
            return false;
        }
        
        std::vector<float> data1(sfinfo1.frames * sfinfo1.channels);
        std::vector<float> data2(sfinfo2.frames * sfinfo2.channels);
        
        sf_readf_float(infile1, data1.data(), sfinfo1.frames);
        sf_readf_float(infile2, data2.data(), sfinfo2.frames);
        
        sf_close(infile1);
        sf_close(infile2);
        
        sfinfo_out = sfinfo1;
        sfinfo_out.frames = sfinfo1.frames + sfinfo2.frames - fade_frames;
        
        if (!(outfile = sf_open(output_file.c_str(), SFM_WRITE, &sfinfo_out))) {
            std::cerr << "Error creating " << output_file << ": " << sf_strerror(nullptr) << std::endl;
            return false;
        }
        
        sf_count_t frames_to_write = sfinfo1.frames - fade_frames;
        sf_writef_float(outfile, data1.data(), frames_to_write);
        
        std::vector<float> crossfade_buffer(fade_frames * sfinfo1.channels);
        for (int i = 0; i < fade_frames; i++) {
            float fade_out = static_cast<float>(fade_frames - i) / fade_frames;
            float fade_in = static_cast<float>(i) / fade_frames;
            
            for (int ch = 0; ch < sfinfo1.channels; ch++) {
                int idx1 = (frames_to_write + i) * sfinfo1.channels + ch;
                int idx2 = i * sfinfo2.channels + ch;
                crossfade_buffer[i * sfinfo1.channels + ch] = 
                    data1[idx1] * fade_out + data2[idx2] * fade_in;
            }
        }
        
        sf_writef_float(outfile, crossfade_buffer.data(), fade_frames);
        sf_writef_float(outfile, data2.data() + (fade_frames * sfinfo2.channels), 
                        sfinfo2.frames - fade_frames);
        
        sf_close(outfile);
        return true;
    }

    std::string find_reference_audio_file() {
        // Look for any .wav file in the AUDIO_FOLDER as a reference
        std::string command = "ls " + std::string(AUDIO_FOLDER) + "*.wav | head -n 1";
        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) {
            std::cerr << "Failed to find reference audio file." << std::endl;
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
    }

    bool add_natural_pause(const std::string& input_file, const std::string& output_file, float pause_sec = 0.02f) {
        std::string samples = std::to_string(static_cast<int>(pause_sec * 24000));
        
        std::string command = "sox " + input_file + " " + output_file + 
                              " pad 0 " + samples + "s";
        
        int result = system(command.c_str());
        if (result != 0) {
            std::cerr << "Error adding pause to " << input_file << std::endl;
            return false;
        }
        return true;
    }

    unsigned int detect_sample_rate(const std::string& file) {
        SF_INFO sfinfo;
        memset(&sfinfo, 0, sizeof(sfinfo));
        
        SNDFILE* infile = sf_open(file.c_str(), SFM_READ, &sfinfo);
        if (!infile) {
            std::cerr << "Error opening " << file << ": " << sf_strerror(nullptr) << std::endl;
            return 44100; // Default sample rate
        }

        unsigned int rate = sfinfo.samplerate;
        sf_close(infile);
        return rate;
    }

    void ensure_silence_files() {
        std::string reference_file = find_reference_audio_file();
        
        if (reference_file.empty()) {
            std::cout << "No reference audio file found. Using default sample rate of 44100 Hz." << std::endl;
            sample_rate = 44100;
        } else {
            sample_rate = detect_sample_rate(reference_file);
            std::cout << "Using reference file: " << reference_file << std::endl;
            std::cout << "Detected sample rate: " << sample_rate << " Hz" << std::endl;
        }

        std::string silence_dir(SILENCE_FOLDER);
        system(("mkdir -p " + silence_dir).c_str());
        
        // Create silence files of different durations with the matching sample rate
        std::map<int, std::string> silence_files = {
            {1, silence_dir + "silence_1.wav"},
            {2, silence_dir + "silence_2.wav"},
            {3, silence_dir + "silence_3.wav"},
            {4, silence_dir + "silence_4.wav"}
        };
        
        for (const auto& pair : silence_files) {
            std::ifstream file(pair.second);
            float duration = 0.1f * pair.first;  // 0.1s per unit
            
            // Always recreate the silence files to ensure they have the correct sample rate
            std::string command = "sox -n -r " + std::to_string(sample_rate) + " " + pair.second + " trim 0.0 " + std::to_string(duration);
            system(command.c_str());
            std::cout << "Created silence file: " + pair.second + " with sample rate " + std::to_string(sample_rate) + " Hz" << std::endl;
        }
    }

    bool combine_audio_files(const std::vector<std::string>& input_files, const std::string& output_path, float crossfade_time = 0.03f) {
        if (input_files.empty()) {
            std::cerr << "No input files provided!" << std::endl;
            return false;
        }

        if (input_files.size() == 1) {
            std::string command = "cp \"" + input_files[0] + "\" \"" + output_path + "\"";
            system(command.c_str());
            return true;
        }

        // Use sox to combine all files at once
        std::string command = "sox";
        for (const auto& file : input_files) {
            command += " \"" + file + "\"";
        }
        command += " \"" + output_path + "\"";
        
        int result = system(command.c_str());
        if (result != 0) {
            std::cerr << "Error combining audio files. Command: " << command << std::endl;
            return false;
        }

        std::cout << "Audio files successfully combined into " << output_path << std::endl;
        return true;
    }

    std::string get_silence_file(char punctuation) {
        std::string silence_dir(SILENCE_FOLDER);
        
        switch (punctuation) {
            case ' ': return silence_dir + "silence_1.wav"; // Space: 1 unit delay
            case ',': return silence_dir + "silence_2.wav"; // Comma: 2 unit delay
            case ';': return silence_dir + "silence_3.wav"; // Semicolon: 3 unit delay
            case '.': return silence_dir + "silence_4.wav"; // Period: 4 unit delay
            case '?': return silence_dir + "silence_4.wav"; // Question mark: 4 unit delay
            case ':': return silence_dir + "silence_3.wav"; // Colon: 3 unit delay
            case '"': return silence_dir + "silence_2.wav"; // Quotation mark: 2 unit delay
            default: return silence_dir + "silence_1.wav";  // Default: 1 unit delay
        }
    }

    std::vector<std::string> split_sentence(const std::string& sentence) {
        std::vector<std::string> words;
        std::istringstream stream(sentence);
        std::string word;
        while (stream >> word) {
            words.push_back(word);
        }
        return words;
    }

    std::string to_lowercase(const std::string& str) {
        std::string lower_str = str;
        std::transform(lower_str.begin(), lower_str.end(), lower_str.begin(), ::tolower);
        return lower_str;
    }

    std::string remove_punctuation(const std::string& sentence) {
        std::string result;
        for (char ch : sentence) {
            if (std::isalnum(ch) || std::isspace(ch)) {
                result += ch;
            }
        }
        return result;
    }
};

#endif



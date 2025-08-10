

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>
#include <string>
#include <regex>
#include <chrono>
#include "basic_services.h"
#include <unordered_map>
#include <sstream>
#include <cstdlib>
#include "db_driver1.h"
#include "profiles.h"
#include "call_manager_copy.h"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

const std::string AUDIO_DIR = "ver_feb_07/audio/";
const std::string UPLOAD_ROUTE = "/upload-audio";
basic_services bs(5);
std::unordered_map<std::string, std::string> session_data;
CallManager* manager_;

void create_directories() {
    if (!std::filesystem::exists(AUDIO_DIR)) {
        std::filesystem::create_directories(AUDIO_DIR);
    }
}

std::string url_decode(const std::string& str) {
    std::ostringstream decoded;
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '%' && i + 2 < str.length()) {
            int hex;
            sscanf(str.substr(i + 1, 2).c_str(), "%x", &hex);
            decoded << static_cast<char>(hex);
            i += 2;
        } else if (str[i] == '+') {
            decoded << ' ';
        } else {
            decoded << str[i];
        }
    }
    return decoded.str();
}



void handle_specific_request(http::request<http::string_body> req, http::response<http::string_body>& res) {
    std::string target = std::string(req.target());

    if (target == "/first_question") {
        auto [state, message] = manager_->get_first_question();
        std::string bot_filename = "audio1.wav";
        bs.to_voice_folder(message, AUDIO_DIR + bot_filename);

        res.result(http::status::ok);
        res.set(http::field::content_type, "application/json");
        res.set(http::field::access_control_allow_origin, "*");
        res.body() = R"({"message":")" + message + R"(","audio":"audio1.wav"})";
    }
    else if (target.find("/get_next_question/") == 0) {
        std::string encodedInput = target.substr(18);
        std::string userInput = url_decode(encodedInput);
        if (!userInput.empty() && userInput[0] == '/') userInput = userInput.substr(1);

        std::string response = manager_->runChatbot(userInput);
        std::string bot_filename = "audio2.wav";
        bs.to_voice_folder(response, AUDIO_DIR + bot_filename);

        res.result(http::status::ok);
        res.set(http::field::content_type, "application/json");
        res.set(http::field::access_control_allow_origin, "*");
        res.body() = R"({"message":")" + response + R"(","audio":"audio2.wav"})";
    }
    else if (target == "/is_last_state") {
        bool isLastState = manager_->isLastState();
        res.result(http::status::ok);
        res.set(http::field::content_type, "text/plain");
        res.set(http::field::access_control_allow_origin, "*");
        res.body() = isLastState ? "true" : "false";
    }
    else {
        res.result(http::status::not_found);
        res.set(http::field::content_type, "text/plain");
        res.body() = "404 Not Found";
    }

    res.prepare_payload();
}

void set_cors_headers(http::response<http::string_body>& res) {
    res.set(http::field::access_control_allow_origin, "*");
    res.set(http::field::access_control_allow_methods, "POST, GET, OPTIONS");
    res.set(http::field::access_control_allow_headers, "Content-Type");
}

void handle_post(const http::request<http::string_body>& req, http::response<http::string_body>& res) {
    std::string content_type(req.at(http::field::content_type));
    std::string boundary = content_type.substr(content_type.find("boundary=") + 9);
    std::string body = req.body();

    std::string filename = "audio1.webm";

    size_t start = body.find("\r\n\r\n") + 4;
    size_t end = body.find("--" + boundary);
    if (start != std::string::npos && end != std::string::npos) {
        std::string content = body.substr(start, end - start - 2);
        std::ofstream file(AUDIO_DIR + filename, std::ios::binary);
        if (file.is_open()) {
            file.write(content.c_str(), content.size());
            file.close();
            std::cout << "Saved audio: " << filename << std::endl;

            std::string wav_filename = "audio1.wav";
            bs.convert_webm_to_wav(AUDIO_DIR + filename, AUDIO_DIR + wav_filename);
            std::string text = bs.to_text(1, AUDIO_DIR + wav_filename);
            std::cout << "Recognized text: " << text << std::endl;

            res.body() = R"({"status":"success","recognizedText":")" + text + "\"}";
            res.result(http::status::ok);
        }
    }
    res.set(http::field::content_type, "application/json");
}

void handle_get(const http::request<http::string_body>& req, http::response<http::string_body>& res) {
    //std::string path = req.target().to_string();
    std::string path = std::string(req.target());
    std::string filepath = AUDIO_DIR + path.substr(path.find_last_of('/') + 1);

    if (std::filesystem::exists(filepath)) {
        std::ifstream file(filepath, std::ios::binary);
        res.body() = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
        res.set(http::field::content_type, "audio/wav");
        res.result(http::status::ok);
    } else {
        res.result(http::status::not_found);
    }
}

void handle_request(const http::request<http::string_body>& req, http::response<http::string_body>& res) {
    set_cors_headers(res);

    if (req.method() == http::verb::options) {
        res.result(http::status::ok);
        return;
    }

    if (req.method() == http::verb::post && req.target() == UPLOAD_ROUTE) {
        handle_post(req, res);
    } else if (req.method() == http::verb::get && req.target().starts_with("/ver_feb_07/audio/")) {
        handle_get(req, res);
    } else {
        handle_specific_request(req, res);
    }
}

void session(tcp::socket socket) {
    try {
        beast::flat_buffer buffer;
        http::request<http::string_body> req;
        http::read(socket, buffer, req);

        http::response<http::string_body> res;
        handle_request(req, res);

        http::write(socket, res);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

int main() {
    manager_ = new CallManager("hanana", "root", "127.0.0.1", "3306", "Final", 10, 5);

    create_directories();
    try {
        net::io_context ioc;
        tcp::acceptor acceptor(ioc, tcp::endpoint(tcp::v4(), 8080));
        std::cout << "Server is running on port 8080..." << std::endl;

        while (true) {
            tcp::socket socket(ioc);
            acceptor.accept(socket);
            std::thread(session, std::move(socket)).detach();
        }
    } catch (std::exception const& e) {
        std::cerr << "Critical Error: " << e.what() << std::endl;
    }

    delete manager_;
    return 0;
}



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
#include <cstdlib>  // For std::getenv
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

std::string getEnv(const char* name, const std::string& defaultValue = "") {
    const char* value = std::getenv(name);
    return value ? value : defaultValue;
}

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

// --- Your handle_specific_request, set_cors_headers, handle_post, handle_get, handle_request, session stay unchanged ---

int main() {
    // Get DB credentials from environment variables
    std::string db_user = getEnv("DB_USER", "root");
    std::string db_pass = getEnv("DB_PASSWORD", "");
    std::string db_host = getEnv("DB_HOST", "127.0.0.1");
    std::string db_port = getEnv("DB_PORT", "3306");
    std::string db_name = getEnv("DB_NAME", "Final");

    manager_ = new CallManager(
        db_user,
        db_pass,
        db_host,
        db_port,
        db_name,
        10, 5
    );

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

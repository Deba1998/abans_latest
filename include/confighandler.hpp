#ifndef CONFIG_READER_H
#define CONFIG_READER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <stdexcept>

// Configuration reader class
class ConfigReader {
private:
    std::unordered_map<std::string, std::string> config;

public:
    // Load configuration from file
    void load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + filename);
        }

        std::string line;
        while (std::getline(file, line)) {
            // Remove comments and trim whitespace
            size_t commentPos = line.find('#');
            if (commentPos != std::string::npos) {
                line = line.substr(0, commentPos);
            }
            line = trim(line);

            // Skip empty lines
            if (line.empty()) continue;

            // Parse key-value pairs
            size_t delimiterPos = line.find('=');
            if (delimiterPos == std::string::npos) {
                throw std::runtime_error("Invalid line in config file: " + line);
            }

            std::string key = trim(line.substr(0, delimiterPos));
            std::string value = trim(line.substr(delimiterPos + 1));
            config[key] = value;
        }

        file.close();
    }

    // Get value by key
    std::string get(const std::string& key) const {
        auto it = config.find(key);
        if (it == config.end()) {
            throw std::runtime_error("Key not found: " + key);
        }
        return it->second;
    }

    // Check if a key exists
    bool hasKey(const std::string& key) const {
        return config.find(key) != config.end();
    }

private:
    // Utility function to trim whitespace
    static std::string trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t");
        size_t last = str.find_last_not_of(" \t");
        return (first == std::string::npos || last == std::string::npos)
                   ? ""
                   : str.substr(first, last - first + 1);
    }
};

// int main() {
//     ConfigReader config;

//     try {
//         config.load("config.txt");
//         std::cout << "Host: " << config.get("host") << "\n";
//         std::cout << "Port: " << config.get("port") << "\n";
//         std::cout << "Username: " << config.get("username") << "\n";
//         // Optional check before fetching keys
//         if (config.hasKey("password")) {
//             std::cout << "Password: " << config.get("password") << "\n";
//         }
//     } catch (const std::exception& e) {
//         std::cerr << "Error: " << e.what() << "\n";
//     }

//     return 0;
// }

#endif

#ifndef LOGHANDLER_HPP
#define LOGHANDLER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <mutex>
#include <memory>
#include <chrono>
#include <iomanip>

// Log levels
enum class LogLevel {
    ERROR,
    WARNING,
    INFO,
    DEBUG
};

class LogHandler {
public:
    // Static method to get the singleton instance
    static LogHandler& getInstance() {
        static LogHandler instance;
        return instance;
    }

    // Set log level
    void setLogLevel(LogLevel level) {
        logLevel = level;
    }

    // Variadic log function to print multiple messages
    template <typename... Args>
    void log(LogLevel level, Args&&... args) {
        if (level <= logLevel) {
            std::lock_guard<std::mutex> lock(mtx);
            std::stringstream logMessage;

            // Prepend the current date-time to the log message
            logMessage << getCurrentDateTime() << " ";

            // Prepend the log level
            logMessage << getLogPrefix(level);

            // Add the actual log message
            (logMessage << ... << std::forward<Args>(args));  // Expand the arguments into the stream
            std::string finalMessage = logMessage.str();

            // Write the log to file or stdout
            if (logFile.is_open()) {
                logFile << finalMessage << std::endl;
            } else {
                std::cout << finalMessage << std::endl;
            }
        }
    }

    // To initialize the log file (optional)
    void initializeLogFile(const std::string& filename) {
        logFile.open(filename, std::ios::app);
    }

private:
    // Private constructor for Singleton
    LogHandler() : logLevel(LogLevel::DEBUG) {}

    // Get the log level prefix
    std::string getLogPrefix(LogLevel level) {
        switch (level) {
            case LogLevel::ERROR: return "[ERROR] ";
            case LogLevel::WARNING: return "[WARNING] ";
            case LogLevel::INFO: return "[INFO] ";
            case LogLevel::DEBUG: return "[DEBUG] ";
            default: return "[INFO] ";
        }
    }

    // Get the current date and time as a string
    std::string getCurrentDateTime() {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::tm tm = *std::localtime(&time);

        // Format: YYYY-MM-DD HH:MM:SS
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    // Disallow copy and assignment
    LogHandler(const LogHandler&) = delete;
    LogHandler& operator=(const LogHandler&) = delete;

    LogLevel logLevel;
    std::ofstream logFile;
    std::mutex mtx;
};

#endif // LOGHANDLER_HPP

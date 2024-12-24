#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>


std::string getCurrentDate() {
    // Get current system time
    auto now = std::chrono::system_clock::now();
    
    // Convert to time_t
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    
    // Format the date and return as a string
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now_c), "%Y-%m-%d");
    return oss.str();
}

#endif // UTILITY_H
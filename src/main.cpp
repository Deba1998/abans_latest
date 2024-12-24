#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>

#include "confighandler.hpp"
#include "loghandler.hpp"
#include "utility.hpp"
#include "Receiver.hpp"

int main()
{
    std::string logstring="LOGS/"+getCurrentDate()+".log";
    // LogHandler logger(logstring, LogHandler::INFO);
    LogHandler::getInstance().setLogLevel(LogLevel::INFO);
    LogHandler::getInstance().initializeLogFile(logstring);
    
    ConfigReader config;
    try {
        config.load("CONFIGS/config.txt");
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        LogHandler::getInstance().log(LogLevel::ERROR, "Error: ", e.what());
        exit(1);
    }
    std::string serverip,serverport;
    try{
        serverip=config.get("server_ip");
    }
    catch(...)
    {
        LogHandler::getInstance().log(LogLevel::ERROR, "Error: ", "server_ip missing");
        exit(1);
    }
    try{
        serverport=config.get("server_port");
    }
    catch(...)
    {
        LogHandler::getInstance().log(LogLevel::ERROR, "Error: ", "server_port missing");
        exit(1);
    }

    Reciever r(serverip,serverport);
    r.start();
    r.join();

    // sleep(7);
    
    // while(true)
    // {
    //     sleep(100);
    // }

    return 0;
}
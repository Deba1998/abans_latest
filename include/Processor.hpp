#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <string>
#include <vector>
#include <thread>
#include <iostream>
#include <cstring>
#include <map>
#include <chrono>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include "loghandler.hpp"
#include "blockingconcurrentqueue.h"
#include "loghandler.hpp"
#include "ResponseStructure.hpp"
#define BUFFER_SIZE 1024

class Processor
{
    public:
        std::string ip;
        std::string port;
        moodycamel::BlockingConcurrentQueue<std::vector<unsigned char>> q;
        std::thread processorthread;
        std::map<int,marketstruct> mp;
        std::vector<int> missinglist;
        marketstruct ms;
        // bool recievercompleted=false;
        int packetpushed=0;
        int packetprocessed=0;
        Processor(std::string serverip, std::string serverport)
        {
            this->ip=serverip;
            this->port=serverport;
        };
        void enque_processor(std::vector<unsigned char> &v);
        void process();
        void start();
        void collect_missing_packets(int sequencenumber);
        void join();
     
};


#endif // PROCESSOR_H
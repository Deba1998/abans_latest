#ifndef RECIEVER_H
#define RECIEVER_H
#include <string>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include "loghandler.hpp"
#include "Processor.hpp"
#define BUFFER_SIZE 1024


class Reciever
{
    public:
        std::string ip;
        std::string port;
        std::thread recieverthread;
        moodycamel::BlockingConcurrentQueue<std::vector<unsigned char>> q;
        Processor *p;
        Reciever(std::string serverip, std::string serverport)
        {
            this->ip=serverip;
            this->port=serverport;
            p= new Processor(serverip,serverport);
        }
        void run();
        void start();
        void join();
     
};


#endif // RECIEVER_H
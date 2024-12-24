#include "Receiver.hpp"

void Reciever::run()
{
    // Processor p(ip,port);
    // p.start();
    // p.start();
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        LogHandler::getInstance().log(LogLevel::ERROR, "Socket creation failed");
        return;
    }
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(std::stoi(port));
    if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0) {
        LogHandler::getInstance().log(LogLevel::ERROR, "Invalid address or address not supported");
        close(sock);
        return;
    }
    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        LogHandler::getInstance().log(LogLevel::ERROR, "Connection to the server failed");
        close(sock);
        return;
    }
    uint8_t payload[2];
    payload[0] = 1;
    if (send(sock, payload, sizeof(payload), 0) < 0) {
        LogHandler::getInstance().log(LogLevel::ERROR, "Failed to send data");
        close(sock);
        return;
    }
    LogHandler::getInstance().log(LogLevel::INFO, "Request sent successfully");
    unsigned char buffer[BUFFER_SIZE];
    while (true) {
        int bytesReceived = recv(sock, buffer, BUFFER_SIZE, 0);
        if (bytesReceived < 0) {
            LogHandler::getInstance().log(LogLevel::ERROR, "Error receiving data");
            break;
        }
        else if (bytesReceived == 0) {
            p->start();
            p->join();
            LogHandler::getInstance().log(LogLevel::INFO, "Connection closed by server");
            break;
        }
        LogHandler::getInstance().log(LogLevel::INFO, "Bytes received: ", bytesReceived);
        std::vector<unsigned char> vec(buffer, buffer + bytesReceived);
        p->enque_processor(vec);
        std::memset(buffer, 0, sizeof(buffer));

    }
    close(sock);
}

void Reciever::start()
{
    recieverthread = std::thread([this]() { 
            Reciever::run(); 
        }); 
    // recieverthread.detach();
}
void Reciever::join()
{
    recieverthread.join();
}
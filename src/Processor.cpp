#include "Processor.hpp"
using json = nlohmann::json;
template <typename T>
T bswap(T value) {
   if (sizeof(T) == 2) {
        return __builtin_bswap16(value);
    } else if (sizeof(T) == 4) {
        return __builtin_bswap32(value);
    } else if (sizeof(T) == 8) {
        return __builtin_bswap64(value);
    }
}

void to_json(json& j, const marketstruct& data) {
    std::string str(data.symbol, 4);
    j = json{
        {"symbol", str},
        {"BuySell_Indicator", std::string(1, data.buy_sell)}, 
        {"Quantity", data.quantity},
        {"Price", data.price},
        {"Packet_Sequence", data.packetsequence}
    };
}


void parseMarketStruct(const unsigned char* buffer, marketstruct& ms) {
    memcpy(ms.symbol, buffer, 4);
    ms.buy_sell = buffer[4];
    ms.quantity = bswap(*reinterpret_cast<const int*>(buffer + 5));
    ms.price = bswap(*reinterpret_cast<const int*>(buffer + 9));
    ms.packetsequence = bswap(*reinterpret_cast<const int*>(buffer + 13));
}



void Processor::enque_processor(std::vector<unsigned char> &v)
{
    q.enqueue(std::move(v));
    packetpushed++;
}
// void Processor::setrecivercompleted()
// {
//     recievercompleted=true;
// }


void Processor::collect_missing_packets(int sequencenumber)
{
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
    payload[0] = 2; 
    payload[1] = sequencenumber; 
    if (send(sock, payload, sizeof(payload), 0) < 0) {
        LogHandler::getInstance().log(LogLevel::ERROR, "Failed to send data");
        close(sock);
        return;
    }
    LogHandler::getInstance().log(LogLevel::INFO, "Request sent successfully");
    unsigned char buffer[BUFFER_SIZE];
    int bytesReceived = recv(sock, buffer, BUFFER_SIZE, 0);
    if (bytesReceived < 0) {
        LogHandler::getInstance().log(LogLevel::ERROR, "Error receiving data");
        exit(1);
    }
    else if (bytesReceived == 0) {
        LogHandler::getInstance().log(LogLevel::INFO, "Connection closed by server");
    }
    LogHandler::getInstance().log(LogLevel::INFO, "Bytes received for packet specific: ", bytesReceived);

    std::vector<unsigned char> vec(buffer, buffer + bytesReceived);
    parseMarketStruct(vec.data(),ms);
    mp[ms.packetsequence]=ms;
    close(sock);

}

void Processor::process()
{
    int sequencenumber=1;
    while (packetprocessed<packetpushed)
    {
        std::vector<unsigned char> vec;
        q.wait_dequeue(vec);
        LogHandler::getInstance().log(LogLevel::INFO, "The size of the out queue ",vec.size());
        int total=vec.size();
        int offset=0;
        while(offset<total)
        {
           
           parseMarketStruct(vec.data()+offset,ms);
           if(ms.packetsequence!=sequencenumber){
                while(sequencenumber<ms.packetsequence)
                {
                    missinglist.push_back(sequencenumber);
                    sequencenumber++;
                }
           }
           mp[ms.packetsequence]=ms;
           sequencenumber++;
           offset+=17;
        }
        packetprocessed++;
    }
    for(auto &it:missinglist)
    {
        Processor::collect_missing_packets(it);
    }
    std::vector<marketstruct> marketDataList;
    for(auto &it:mp)
    {
       marketDataList.push_back(it.second);
    }
    json jsonArray = marketDataList;
    std::ofstream outFile("market_data.json");
    if (outFile.is_open()) {
        outFile << jsonArray.dump(4);
        outFile.close();
        LogHandler::getInstance().log(LogLevel::INFO, "JSON saved to market_data.json");
    } else {
        LogHandler::getInstance().log(LogLevel::ERROR, "Failed to open file for writing.");
    }
}

void Processor::start()
{
    processorthread = std::thread([this]() { 
            Processor::process(); 
        }); 
    // processorthread.detach();
}

void Processor::join()
{
    processorthread.join();
}
# abans_latest
Folder structure:-
bin:- contains the executable
CONFIGS:- to hold the configuration file
include:- to hold the include files
LOGS:- folder where logs are getting genrated with the corresponding date
src:- Contains the main program files
Makefile:- To compile the program
market_data.json:- the resultant json file after processing.

open source dependencies/requirements:-
moody camel queue to interact between threads(no installation required header files is in include folder)
nlohmann/json.hpp (for json object parsing)(installation command in ubuntu:- sudo apt install nlohmann-json3-dev)

running instruction:-
clone the repository
cd to folder abans_latest
run ./bin/abans
check the market_data.json file after compilation

Approach:-
so here we are using two threads reciever and processor.
reciever thread is used for recieving all packets from the server initial call after that it enques to the moodycamel queue. 
after reciever is completed processor starts it deque the packets and then parse and store it in resultant map and make a list of missing packets.
Then the missing packets is collected and added to the resultant map and then the final json is wrote and the program completed

#note:- this multithreading approach can be extended to recieve and process the packets parallely with proper synchronization mechanism i have tried one approach using flags but due to lack of time it is not possible to implement fully i have made the program to run the reciever and processor serially in the current implementation

Assumptions:-

Parital packets is not recived from the server



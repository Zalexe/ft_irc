#include <iostream>
#include "Server.hpp"
#include "Utils.hpp"

int main(int argc, char** argv){

    if(argc != 3){
        std::cerr << "Usage is: ./ircserver <port> <passwword>" << std::endl;
        std::exit(1);
    }
    int port = atoi(argv[1]);
    std::string password = argv[2];
    if (!port || port > 65,535 || port< 1024 || password.empty()){
        std::cerr << "Invalid arguments" << std::endl;
        std::exit(1);
    }

    return 0;
}
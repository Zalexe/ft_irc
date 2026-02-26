#pragma once

#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <typeinfo>
#include <iostream>
#include <cerrno>
#include <vector>

#include "Channel.hpp"
#include "Client.hpp"

#define SERVER_NAME "ft_irc"

class Server{
    private:
        std::vector<Channel*> _channels;
        std::vector<Client*> _clients;
        Client* _serverClient;
        std::string _pass, _serverPrefix, _message;
	    int _serverSocket, _epollSocket, _port;
    public:
        Server();
        Server(int Port, std::string Pass);
        ~Server();

        void run();
        void acceptClient();
        void handleClient(int fd);
        void sendWelcome(Client* client);
        void disconnectClient(int fd);
        void processCommand(Client* client, std::string line);
        Client* getClientByFd(int fd);
        
};

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
#define SERVER_VERSION "1.0"
#define SERVER_DESCRIPTION "IRC Server for 42 by cmarrued & intherna"

#define WELCOME_MESSAGE "Welcome to ft_irc server, " // Host to be appended to the end

/* TODO:
* Because of IRC's Scandinavian origin, the characters {}|^ are
* considered to be the lower case equivalents of the characters []\~,
* respectively. This is a critical issue when determining the
* equivalence of two nicknames or channel names.
*/

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

        std::string extractCommand(const std::string& line);
        void processCommand(Client* client, std::string line);
        void handleRegistration(Client* client, const std::string& command, const std::string& line);
        void executeCommand(Client* client, const std::string& command, const std::string& line);
        Client* getClientByFd(int fd);
        
};

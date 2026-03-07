#pragma once

#include <ctime>
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
#include "Messages.hpp"
#include "Client.hpp"

#define SERVER_NAME "ft_irc"
#define SERVER_VERSION "1.0"
#define SERVER_DESCRIPTION "IRC Server for 42 by cmarrued & intherna"

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
		const std::time_t _creationDate;

		// Handler helpers
        void handleRegistration(Client* client, const std::string& command, const std::string& line);
        void handleJoin(Client* client, std::stringstream stream);
        void handlePrivmsg(Client* client, std::stringstream& line);
        void handleQuit(int ClientFd, const std::string& reason);
        void handleKick(Client* client, std::stringstream& line);
        void handleInvite(Client* client, std::stringstream& line);
        void handleTopic(Client* client, std::stringstream& line);
        void handleMode(Client* client, std::stringstream& line);
    public:
        Server();
        Server(int Port, std::string Pass);
        ~Server();

        //main loop
        void run();
        //detect new clients or handle client
        void acceptClient();

        //process commands received
        std::string extractCommand(const std::string& line);
        void processCommand(Client* client, std::string line);
        void executeCommand(Client* client, const std::string& command, const std::string& line);

        //read message and send to appropiate handle
        void handleClient(int fd);
        void disconnectClient(int fd);

        //handle registration helpers
        void sendWelcome(Client* client);
        void tryRegister(Client* client);
        void handlePass(Client* client, const std::string& line);
        std::string toLower(const std::string& str);
        bool nickExists(const std::string& nick, Client* requester);
        void handleNick(Client* client, const std::string& line);
        void handleUser(Client* client, const std::string& line);

        //send error
        void sendError(Client* client, const std::string& msg);

        //accessors
        Client* getClientByFd(int fd) const;
        Client* getClientByName(const std::string& name) const;
		Channel* getChannelByName(const std::string& name) const;
        const std::time_t& getCreationDate() const;
};

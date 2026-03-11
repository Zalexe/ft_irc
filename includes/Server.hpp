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
#include <signal.h>

#include "Bot.hpp"
#include "Client.hpp"
#include "Channel.hpp"

#define SERVER_NAME "ft_irc"
#define SERVER_VERSION "1.0"
#define SERVER_DESCRIPTION "IRC Server for 42 by cmarrued & intherna"

extern bool server_signal;

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
		const Bot bot;

		// Handler helpers
        void handleRegistration(Client* client, const std::string& command, std::stringstream& line);
        void handleJoin(Client* client, std::stringstream& params);
        void handlePart(Client* client, std::stringstream& params);
		void handleWho(Client* client, std::stringstream& params);
        void handlePrivmsg(Client* client, std::stringstream& params);
        void handleQuit(Client* client, const std::string& reason);
        void handleKick(Client* client, std::stringstream& params);
        void handleInvite(Client* client, std::stringstream& params);
        void handleTopic(Client* client, std::stringstream& params);
        void handleMode(Client* client, std::stringstream& params);

        //process commands received
        std::string extractCommand(std::stringstream& ss);
        void processCommand(Client* client, const std::string& line);
        void executeCommand(Client* client, const std::string& command, std::stringstream& params);
    public:
        Server();
        Server(int Port, std::string Pass);
        ~Server();

        //main loop
        void run();
        //detect new clients or handle client
        void acceptClient();

        //read message and send to appropiate handle
        void handleClient(int fd);

        //handle registration helpers
        void sendWelcome(Client* client);
        void tryRegister(Client* client);
        void handlePass(Client* client, std::stringstream& line);
        bool nickExists(const std::string& nick, Client* requester);
        void handleNick(Client* client, std::stringstream& line);
        void handleUser(Client* client, std::stringstream& line);


        //handle join help
        void sendNamesList(Client* client, Channel* ch);

        //accessors
        Client* getClientByFd(int fd) const;
        Client* getClientByName(const std::string& name) const;
		Channel* getChannelByName(const std::string& name) const;
        
        const std::time_t& getCreationDate() const;
};

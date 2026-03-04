#include "Server.hpp"

void handlePrivmsg(Client* client, const std::string& line) {
	
}

void handleQuit(int ClientFd) {

}

void handleKick(Client* client, const std::string& line) {

}

void handleInvite(Client* client, const std::string& line);

void handleTopic(Client* client, const std::string& line);

void handleMode(Client* client, const std::string& line) {
	switch ()
}

void sendError(Client* client, const std::string& msg) {
	send(client->getFd(), msg.c_str(), msg.length(), 0);
}

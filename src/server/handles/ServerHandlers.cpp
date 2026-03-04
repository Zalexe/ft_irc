#include "Client.hpp"
#include <string>

void sendError(Client* client, const std::string& msg) {
	send(client->getFd(), msg.c_str(), msg.length(), 0);
}

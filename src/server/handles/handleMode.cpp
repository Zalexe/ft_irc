#include <cctype>
#include <cstddef>
#include <sstream>
#include <sys/socket.h>
#include "Messages.hpp"
#include "Server.hpp"

// TODO
void Server::handleMode(Client* client, const std::string& line) {
	std::stringstream linestream(line);
	std::string dummy;
	std::string target;
	dummy.reserve(4);

	linestream >> dummy >> target;
	if (target.length() == 0) {
		sendError(client, buildResponseNeedMoreParams(client->nickname.c_str(), "MODE"));
		return;
	} else if (target[0] != '#') {
		sendError(client, buildResponseNoSuchChannel(client->nickname.c_str(), target.c_str()));
		return;
	}
	target.erase(0);

	Channel* ch = NULL;

	for (size_t i = 0; i < _channels.size(); i++) {
		if (_channels[i]->getName() == target) {
			ch = _channels[i];
			break;
		}
	}

	if (!ch) {
		sendError(client, buildResponseNoSuchChannel(client->nickname.c_str(), target.c_str()));
		return;
	} else if (!ch->isOperator(client)) {
		sendError(client, buildResponseNoPrivileges(client->nickname.c_str()));
		return;
	}

	std::string modes;
	linestream >> modes;
	if (modes.length() < 2) {
		sendError(client, buildResponseNeedMoreParams(client->nickname.c_str(), "MODE"));
		return;
	}

	const char* m = modes.c_str();
	bool value = m[0] != '-';

	while (*m && !isalpha(*m))
		m++;

	while (*m) {
		switch (*m) {
			case 'i': ch->setInviteOnly(value); break;
			case 't': ch->setTopicRestricted(value); break;
			case 'i': ch->set(value); break;
			case 'i': ch->setInviteOnly(value); break;
			case 'i': ch->setInviteOnly(value); break;
			case 'i': ch->setInviteOnly(value); break;
			case 'i': ch->setInviteOnly(value); break;
			default: {
				sendError(client, buildResponseUnknownChannelMode(client->nickname.c_str(), *m));
				return;
			}
		}
		m++;
	}
	
	linestream >> dummy >> target;
}

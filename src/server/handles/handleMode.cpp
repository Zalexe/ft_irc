#include <cctype>
#include <cstddef>
#include <sstream>
#include <sys/socket.h>
#include "Messages.hpp"
#include "Server.hpp"

static inline bool handleSetPass(Channel& channel, bool value, std::stringstream& line, std::string& response, const Client& client) {
	if (!value) {
		channel.removeKey();
	} else {
		std::string param;
		line >> param;

		if (param.length() == 0) {
			response = buildResponseNeedMoreParams(client.nickname.c_str(), "MODE");
			return false;
		}

		channel.setKey(param);
	}
	return true;
}

static inline bool handleSetOperator(Channel &channel, bool value, std::stringstream& line, std::string& response, const Client& client, const Server& server) {
	std::string param;
	line >> param;

	if (param.length() == 0) {
		response = buildResponseNeedMoreParams(client.nickname.c_str(), "MODE");
		return false;
	}

	Client* target = server.getClientByName(param);
	if (!target) {
		response = buildResponseUserNotInChannel(client.nickname.c_str(), param.c_str(), channel.getName().c_str());
		return false;
	}

	value ? channel.addOperator(target) : channel.removeOperator(target); // Add/Remove operator status

	return true;
}

void Server::handleMode(Client* client, std::stringstream& line) {
	std::string target;

	line >> target;
	if (target.length() == 0) {
		sendError(client, buildResponseNeedMoreParams(client->nickname.c_str(), "MODE"));
		return;
	} else if (target[0] != '#') {
		sendError(client, buildResponseNoSuchChannel(client->nickname.c_str(), target.c_str()));
		return;
	}
	target.erase(0);

	Channel* ch = this->getChannelByName(target);

	if (!ch) {
		sendError(client, buildResponseNoSuchChannel(client->nickname.c_str(), target.c_str()));
		return;
	} else if (!ch->isOperator(client)) { // TODO: Probably should do this earlier
		sendError(client, buildResponseNoPrivileges(client->nickname.c_str()));
		return;
	}

	std::string modes;
	line >> modes;
	if (modes.length() < 2) {
		sendError(client, buildResponseNeedMoreParams(client->nickname.c_str(), "MODE"));
		return;
	}

	const char* m = modes.c_str();
	if (m[0] != '-' && m[0] != '+') {
		sendError(client, buildResponseUnknownChannelMode(client->nickname.c_str(), m[0]));
		return;
	}
	bool value = m[0] == '+';

	while (*m && !isalpha(*m))
		m++;

	bool valid = true;
	std::string message;

	while (*m) {
		if (!valid) {
			sendError(client, message);
			return;
		}
		switch (*m) {
			case 'i': ch->setInviteOnly(value); break;
			case 't': ch->setTopicRestricted(value); break;
			case 'k': valid = handleSetPass(*ch, value, line, message, *client); break;
			case 'o': valid = handleSetOperator(*ch, value, line, message, *client, *this); break;
			case 'l': ch->setInviteOnly(value); break;
			default: {
				sendError(client, buildResponseUnknownChannelMode(client->nickname.c_str(), *m));
				return;
			}
		}
		m++;
	}
}

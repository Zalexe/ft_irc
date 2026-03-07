#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <sstream>
#include <sys/socket.h>
#include "Messages.hpp"
#include "Server.hpp"

static inline bool handleSetPass(Channel& channel, bool value, std::stringstream& params, std::string& response, const Client& client) {
	if (!value) {
		channel.removeKey();
	} else {
		std::string param;
		params >> param;

		if (param.length() == 0) {
			response = buildResponseNeedMoreParams(client.nickname.c_str(), "MODE");
			return false;
		}

		channel.setKey(param);
	}
	return true;
}

static inline bool handleSetOperator(Channel &channel, bool value, std::stringstream& params, std::string& response, const Client& client, const Server& server) {
	std::string param;
	params >> param;

	if (param.length() == 0) {
		response = buildResponseNeedMoreParams(client.nickname.c_str(), "MODE");
		return false;
	}

	Client* target = server.getClientByName(param);
	if (!target) {
		response = buildResponseUserNotInChannel(client.nickname.c_str(), param.c_str(), channel.getName().c_str());
		return false;
	}

	value ? channel.addOperator(target) : channel.removeOperator(target);

	return true;
}

static inline bool handleSetUserLimit(Channel& channel, bool value, std::stringstream& params, std::string& response, const Client& client) {
	if (!value)
		channel.removeUserLimit();
	else {
		std::string param;
		params >> param;
		size_t limit = atoll(param.c_str());
		
		if (limit < 1) {
			response = buildResponseNeedMoreParams(client.nickname.c_str(), "MODE");
			return false;
		}

		channel.setUserLimit(limit);
	}
	return true;
}

void Server::handleMode(Client* client, std::stringstream& params) {
	std::string target;

	params >> target;
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
	} else if (!ch->isOperator(client)) {
		sendError(client, buildResponseChannelOpNeeded(client->nickname.c_str(), ch->getName().c_str()));
		return;
	}

	std::string modes;
	params >> modes;
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
			case 'k': valid = handleSetPass(*ch, value, params, message, *client); break;
			case 'o': valid = handleSetOperator(*ch, value, params, message, *client, *this); break;
			case 'l': valid = handleSetUserLimit(*ch, value, params, message, *client); break;
			default: {
				sendError(client, buildResponseUnknownChannelMode(client->nickname.c_str(), *m));
				return;
			}
		}
		m++;
	}
}

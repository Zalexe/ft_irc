#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <sstream>
#include <sys/socket.h>
#include "Messages.hpp"
#include "Server.hpp"

static inline bool handleSetPass(Channel& channel, bool value, std::stringstream& params, std::string& response, const Client& client, std::string& changesOut, std::string& paramsOut) {
	std::string param;
	params >> param;
	if (param.length() == 0) {
		response = buildResponseNeedMoreParams(client.nickname.c_str(), "MODE");
		return false;
	} else if (!value) {
		if (param != channel.getKey())
			return false;
		channel.removeKey();
	} else {
		if (channel.hasKey()) {
			response = buildResponseCodeMessage(2, ERR_KEYSET, channel.getName().c_str(), "Channel already set");
			return false;
		}
		channel.setKey(param);
	}
	changesOut.push_back('k');
	paramsOut.append(" ").append(param);
	return true;
}

static inline bool handleSetOperator(Channel &channel, bool value, std::stringstream& params, std::string& response, const Client& client, const Server& server, std::string& changesOut, std::string& paramsOut) {
	std::string param;
	params >> param;

	if (param.length() == 0) {
		response = buildResponseNeedMoreParams(client.nickname.c_str(), "MODE");
		return false;
	}

	Client* target = server.getClientByName(param);
	if (!target || !channel.isMember(target)) {
		response = buildResponseUserNotInChannel(client.nickname.c_str(), param.c_str(), channel.getName().c_str());
		return false;
	}

	if (channel.isOperator(target) != value) {
		changesOut.push_back('o');
		paramsOut.append(" ").append(param);
	}
	value ? channel.addOperator(target) : channel.removeOperator(target);
	return true;
}

static inline bool handleSetUserLimit(Channel& channel, bool value, std::stringstream& params, std::string& response, const Client& client, std::string& changesOut, std::string& paramsOut) {
	if (!value) {
		if (channel.getUserLimit() > 0)
			changesOut.push_back('l');
		channel.removeUserLimit();
	} else {
		std::string param;
		params >> param;
		size_t limit = atoll(param.c_str());
		
		if (limit < 1) {
			response = buildResponseNeedMoreParams(client.nickname.c_str(), "MODE");
			return false;
		}

		changesOut.push_back('l');
		paramsOut.append(" ").append(param);
		channel.setUserLimit(limit);
	}
	return true;
}

void Server::handleMode(Client* client, std::stringstream& params) {
	std::string target;

	// Channel param check
	params >> target;
	if (target.length() == 0) {
		sendMessage(client, buildResponseNeedMoreParams(client->nickname.c_str(), "MODE"));
		return;
	} else if (target[0] != '#') {
		sendMessage(client, buildResponseNoSuchChannel(client->nickname.c_str(), target.c_str()));
		return;
	}

	Channel* ch = this->getChannelByName(target);

	if (!ch) {
		sendMessage(client, buildResponseNoSuchChannel(client->nickname.c_str(), target.c_str()));
		return;
	}

	std::string modes;
	params >> modes;
	if (modes.length() == 0) {
		sendMessage(client, buildResponseChannelModeIs(*client, *ch));
		if (!ch->getInvites().empty())
			sendMessage(client, buildResponsesInviteList(client->nickname.c_str(), *ch));
		return;
	} else if (modes.length() < 2) {
		sendMessage(client, buildResponseNeedMoreParams(client->nickname.c_str(), "MODE"));
		return;
	} else if (!ch->isOperator(client)) {
		sendMessage(client, buildResponseChannelOpNeeded(client->nickname.c_str(), ch->getName().c_str()));
		return;
	}

	// Modes parsing
	const char* m = modes.c_str();
	if (m[0] != '-' && m[0] != '+') {
		sendMessage(client, buildResponseUnknownChannelMode(client->nickname.c_str(), m[0], ch->getName().c_str()));
		return;
	}
	bool value = m[0] == '+';

	while (*m && !isalpha(*m))
		m++;

	bool valid = true;
	std::string message;
	std::string changesOutput;
	std::string paramsOutput;

	while (*m) {
		if (!valid) {
			sendMessage(client, message);
			break;
		}
		switch (*m) {
			case 'i': {
				if (ch->isInviteOnly() != value)
					changesOutput.push_back('i');
				ch->setInviteOnly(value);
				break;
			}
			case 't': {
				if (ch->isTopicRestricted() != value)
					changesOutput.push_back('t');
				ch->setTopicRestricted(value);
				break;
			}
			case 'k': valid = handleSetPass(*ch, value, params, message, *client, changesOutput, paramsOutput); break;
			case 'o': valid = handleSetOperator(*ch, value, params, message, *client, *this, changesOutput, paramsOutput); break;
			case 'l': valid = handleSetUserLimit(*ch, value, params, message, *client, changesOutput, paramsOutput); break;
			default: {
				sendMessage(client, buildResponseUnknownChannelMode(client->nickname.c_str(), *m, ch->getName().c_str()));
				return;
			}
		}
		m++;
	}

	// Broadcast changes
	ch->broadcast(buildMessageNoTrail(
		2,
		client->toString().c_str(),
		"MODE",
		ch->getName().c_str(),
		((value ? "+" : "-") + changesOutput + paramsOutput).c_str()
	));
}

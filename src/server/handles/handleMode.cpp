#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <sstream>
#include <sys/socket.h>
#include "Messages.hpp"
#include "Server.hpp"

struct ModeData {
	const char mode;
	std::string param; // Nullable
	
	ModeData(char mode) : mode(mode) {}
};

static inline bool handleSetPass(Channel& channel, const ModeData& data, bool value, Client* client, const Server& server) {
	if (!value) {
		channel.removeKey();
	} else {
		if (data.param.length() == 0) {
			server.sendMessage(client, buildResponseNeedMoreParams(client->nickname.c_str(), "MODE"));
			return false;
		}

		channel.setKey(data.param);
	}
	return true;
}

static inline bool handleSetOperator(Channel &channel, const ModeData& data, bool value, Client* client, const Server& server) {
	if (data.param.length() == 0) {
		server.sendMessage(client, buildResponseNeedMoreParams(client->nickname.c_str(), "MODE"));
		return false;
	}

	Client* target = server.getClientByName(data.param);
	if (!target || !channel.isMember(target)) {
		server.sendMessage(client, buildResponseUserNotInChannel(client->nickname.c_str(), data.param.c_str(), channel.getName().c_str()));
		return false;
	}

	value ? channel.addOperator(target) : channel.removeOperator(target);

	return true;
}

static inline bool handleSetUserLimit(Channel& channel, const ModeData& data, bool value, Client* client, const Server& server) {
	if (!value)
		channel.removeUserLimit();
	else {
		size_t limit = atoll(data.param.c_str());
		
		if (limit < 1) {
			server.sendMessage(client, buildResponseNeedMoreParams(client->nickname.c_str(), "MODE"));
			return false;
		}

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
		if(ch->getInvites().size() > 0)
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
	std::vector<ModeData> datas;
	std::vector<ModeData> mods;
	datas.reserve(modes.length() - 1);
	mods.reserve(modes.length() - 1);

	for (size_t i = 0; m[i]; i++) {
		if (value && (m[i] == 'k' || m[i] == 'l')) {
			ModeData data(m[i]);
			params >> data.param;

			datas.push_back(data);
		} else if (m[i] != 'o') {
			datas.push_back(ModeData(m[i]));
		}
	}

	for (size_t i = 0; i < datas.size(); i++) {
		if (!valid)
			break;
		switch (m[i]) {
			case 'i': ch->setInviteOnly(value); break;
			case 't': ch->setTopicRestricted(value); break;
			case 'k': {
				valid = handleSetPass(*ch, datas[i], value, client, *this);
				if (valid)
					mods.push_back(datas[i]);
				break;
			}
			case 'o': {
				valid = handleSetOperator(*ch, datas[i], value, client, *this);
				if (valid)
					mods.push_back(datas[i]);
				break;
			}
			case 'l': {
				valid = handleSetUserLimit(*ch, datas[i], value, client, *this);
				if (valid)
					mods.push_back(datas[i]);
				break;
			}
			default: {
				sendMessage(client, buildResponseUnknownChannelMode(client->nickname.c_str(), *m, ch->getName().c_str()));
				break;
			}
		}
	}

	std::string modesModified;
	std::string modParams;
	modesModified.reserve(mods.size());
	for (size_t i = 0; i < mods.size(); i++) {
		modesModified.push_back(mods[i].mode);
		if (value)
			modParams.append(" ").append(mods[i].param);
	}

	ch->broadcast(buildMessageNoTrail(
		2,
		client->toString().c_str(),
		"MODE",
		ch->getName().c_str(),
		((value ? "+" : "-") + modesModified + modParams).c_str())
	);
}

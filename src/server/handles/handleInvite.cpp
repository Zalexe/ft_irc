#include "Messages.hpp"
#include "Server.hpp"

void Server::handleInvite(Client* client, std::stringstream& params) {
	std::string invited;
	std::string channel;

	params >> invited >> channel;
	if (invited.length() == 0 || channel.length() == 0) {
		sendMessage(client, buildResponseNeedMoreParams(client->nickname.c_str(), "INVITE"));
		return;
	}

	Channel* ch = this->getChannelByName(channel);
	if (!ch) {
		sendMessage(client, buildResponseNoSuchChannel(client->nickname.c_str(), channel.c_str()));
		return;
	} else if (!ch->isMember(client)) {
		sendMessage(client, buildResponseNotOnChannel(client->nickname.c_str(), ch->getName().c_str()));
		return;
	} else if (!ch->isOperator(client)) {
		sendMessage(client, buildResponseChannelOpNeeded(client->nickname.c_str(), ch->getName().c_str()));
		return;
	}

	Client* target = this->getClientByName(invited);
	if (!target) {
		sendMessage(client, buildResponseNoSuchNick(client->nickname.c_str(), invited.c_str()));
		return;
	} else if (ch->isMember(target)) {
		sendMessage(client, buildResponseUserAlreadyInChannel(client->nickname.c_str(), target->nickname.c_str(), ch->getName().c_str()));
		return;
	}

	ch->invite(target);
	sendMessage(client, buildResponseInviting(client->nickname.c_str(), target->nickname.c_str(), ch->getName().c_str()));
	sendMessage(target, buildResponseInviting(client->nickname.c_str(), target->nickname.c_str(), ch->getName().c_str()));
}

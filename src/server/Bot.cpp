#include "Bot.hpp"
#include "Messages.hpp"

std::string Bot::processMsg(const std::string& input) const {
	
}

Bot::Bot() : nick("BOT") {}
Bot::Bot(const std::string name) : nick(name) {}
Bot::Bot(const Bot& other) : nick(other.nick) {}
Bot::~Bot() {}

void Bot::onChannelPrivmsg(const Channel& channel, const std::string msg) const {
	std::string privMsg = buildMessage(2, this->nick.c_str(), "PRIVMSG", channel.getName().c_str(), this->processMsg(msg).c_str());

	channel.broadcast(privMsg);
}

Bot& Bot::operator=(const Bot& other) {
	if (this != &other)
		this->nick = other.nick;

	return *this;
}

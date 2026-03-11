#include "Bot.hpp"
#include "Messages.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
Bot::Bot() : nick("BOT") {}
Bot::Bot(const std::string name) : nick(name) {}
Bot::Bot(const Bot& other) : nick(other.nick) {}
/*
** -------------------------------- DESTRUCTOR --------------------------------
*/
Bot::~Bot() {}
/*
** --------------------------------- OVERLOAD ---------------------------------
*/
Bot& Bot::operator=(const Bot& other) {
	if (this != &other)
		this->nick = other.nick;

	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/
void Bot::onChannelPrivmsg(const Channel& channel, const std::string& msg) const {
	std::string reply = this->processMsg(msg);
    if (reply.empty())
		return;
	std::string privMsg = buildMessage(2, this->nick.c_str(), "PRIVMSG", channel.getName().c_str(), reply.c_str());
	channel.broadcast(privMsg);
}

std::string Bot::processMsg(const std::string& input) const
{
    if (input.empty() || input[0] != '!')
        return "";
    std::stringstream line(input);
    std::string command;
    line >> command;

    std::string reply;

	if (command == "!ping")
		reply = "PONG...";
	else if (command == "!time")
		reply = "Current server time: " + getCurrentTime();
	else if (command == "!hello")
		reply = "Hello from ft_irc bot";
	else if (command == "!help")
		reply = "Commands: !ping !time !hello !help";
	else
		return "";
	return reply;
}
/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */

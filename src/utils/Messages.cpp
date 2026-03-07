#include "Messages.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <cstdarg>
#include <ctime>
#include <sstream>

std::string buildMessage(int n, const char* sender, const char* cmd, ...) {
	va_list list;
	va_start(list, cmd);
	std::string str;
	str.reserve(90);

	str.push_back(':');
	str.append(sender);
	str.push_back(' ');
	str.append(cmd);
	for (int i = 0; i < n; i++) {
		str.push_back(' ');
		if ((i + 1) == n)
			str.push_back(':');

		str.append(va_arg(list, const char*));
	}
	str.append("\r\n");
	va_end(list);

	if (str.size() > 512)
		str.resize(512);
	return str;
}

std::string buildQuitMessage(const char* sender, const char* target, const char* reason) {
	return buildMessage(2, sender, "QUIT", target, reason);
}

std::string buildResponseCodeMessage(int n, const char* code, ...) {
	va_list list;
	va_start(list, code);
	std::string str;
	str.reserve(90);

	str.push_back(':');
	str.append(SERVER_NAME);
	str.push_back(' ');
	str.append(code);
	for (int i = 0; i < n; i++) {
		str.push_back(' ');
		if ((i + 1) == n)
			str.push_back(':');

		str.append(va_arg(list, const char*));
	}
	str.append("\r\n");
	va_end(list);

	if (str.size() > 512)
		str.resize(512);

	return str;
}

// Registration (these 4 messages are sent)
std::string buildResponseWelcome(const Client& target) {
	std::string welcome("Welcome to ft_irc server by cmarrued & intherna, ");
	welcome.append(target.toString());

	return buildResponseCodeMessage(2, WELCOME, target.nickname.c_str(), welcome.c_str());
}

std::string buildResponseYourhost(const Client& target) {
	std::string yourHost(SERVER_NAME);
	yourHost.append(" version ").append(SERVER_VERSION);

	return buildResponseCodeMessage(2, YOUR_HOST, target.nickname.c_str(), yourHost.c_str());
}

std::string buildResponseCreationDate(const Client& target, const time_t& time) {
	char buf[60];
	std::tm *local = std::localtime(&time);
	strftime(buf, 60, "This server was created at %Y-%m-%d %H:%M:%S", local);

	return buildResponseCodeMessage(2, SERVER_CREATED, target.nickname.c_str(), buf);
}

std::string buildResponseMyInfo(Client& target) {
	std::string myInfo(SERVER_NAME);
	myInfo.append(" ").append(SERVER_VERSION)
		.append(" ").append(target.getAvailableModes().c_str())
		.append(" ").append(Channel::MODES);

	return buildResponseCodeMessage(2, SERVER_MYINFO, target.nickname.c_str(), myInfo.c_str());
}

// Info
std::string buildResponseWhoisuser(const char* targetNick, const Client& user) {
	std::string finalArg(":");
	finalArg.append(user.realName);

	return buildResponseCodeMessage(6, WHOISUSER, targetNick, user.nickname.c_str(), user.name.c_str(), inet_ntoa(user.getAddr().sin_addr), "*", finalArg.c_str());
}

std::string buildResponseChannelModeIs(Client& target, const Channel& channel) {
	std::string str(":");
	str.reserve(100);
	str.append(SERVER_NAME);
	str.push_back(' ');
	str.append(CHANNEL_MODE_IS);
	str.push_back(' ');
	str.append(target.nickname.c_str());
	str.push_back(' ');
	str.push_back('#');
	str.append(channel.getName());
	str.push_back(' ');
	str.push_back('+');

	std::string params;
	if (channel.isInviteOnly())
		str.push_back('i');
	if (channel.isTopicRestricted())
		str.push_back('t');
	if (channel.isOperator(&target))
		str.push_back('o');
	if (channel.getUserLimit() != 0) {
		str.push_back('l');
		params.reserve(14);
		params.push_back(' ');

		std::ostringstream stream;
		stream << channel.getUserLimit();
		params.append(stream.str());
	}

	str.append(params);
	str.append("\r\n");

	if (str.size() >= 512)
		str.resize(512);

	return str;
}

// Error
std::string buildResponseNoPrivileges(const char* targetNick) {
	return buildResponseCodeMessage(2, NOPRIVILEGES, targetNick, "Permission Denied- You're not an IRC operator");
}

std::string buildResponseChannelOpNeeded(const char* targetNick, const char* channel) {
	return buildResponseCodeMessage(2, CHANNEL_OP_NEEDED, targetNick, channel, "You're not a channel operator");
}

std::string buildResponseNoSuchChannel(const char* targetNick, const char* channel) {
	return buildResponseCodeMessage(3, NOSUCHCHANNEL, targetNick, channel, "No such channel");
}

std::string buildResponseUnknownChannelMode(const char* targetNick, char mode, const char* channel) {
	std::string msg("is unknown mode char to me for ");
	msg.append(channel);

	char arr[2];
	arr[0] = mode;
	arr[1] = '\0';

	return buildResponseCodeMessage(3, UNKNOWN_CHANNEL_MODE, targetNick, arr, msg.c_str());
}

std::string buildResponseNeedMoreParams(const char* targetNick, const char* cmd) {
	return buildResponseCodeMessage(3, NEEDMOREPARAMS, targetNick, cmd, "Not enough parameters");
}

std::string buildResponseUserNotInChannel(const char* targetNick, const char* nickNotFound, const char* channel) {
	return buildResponseCodeMessage(4, USER_NOT_IN_CHANNEL, targetNick, nickNotFound, channel, "They aren't on that channel");
}

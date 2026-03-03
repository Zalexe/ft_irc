#include "Messages.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <cstdarg>
#include <ctime>
#include <stdexcept>

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
		throw std::runtime_error("The message is too large (> 512)");
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
		throw std::runtime_error("The message is too large (> 512)");
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

std::string buildResponseMyInfo(Client& target, const Channel& channel) {
	std::string myInfo(SERVER_NAME);
	myInfo.append(" ").append(SERVER_VERSION)
		.append(" ").append(channel.isOperator(&target) ? Client::OP_MODES : Client::NON_OP_MODES)
		.append(" ").append(Channel::MODES);

	return buildResponseCodeMessage(2, SERVER_MYINFO, target.nickname.c_str(), myInfo.c_str());
}

// Info
std::string buildResponseWhoisuser(const char* targetNick, const Client& user) {
	std::string finalArg(":");
	finalArg.append(user.realName);

	return buildResponseCodeMessage(3, WHOISUSER, targetNick, user.nickname.c_str(), user.name.c_str(), inet_ntoa(user.getAddr().sin_addr), "*", finalArg.c_str());
}

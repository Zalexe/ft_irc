#include "Messages.hpp"
#include "Server.hpp"
#include <cstdarg>
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

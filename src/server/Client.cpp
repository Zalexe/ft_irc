#include "Client.hpp"
#include "Messages.hpp"
#include "Server.hpp"
#include <cstring>
#include <sys/socket.h>
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
Client::Client() : _fd(-1), _alive(false) {}
Client::Client(int fd, struct sockaddr_in host) : _fd(fd), _host(host), _alive(true), _authorized(false), _registered(false) {}
/*
** -------------------------------- DESTRUCTOR --------------------------------
*/
Client::~Client() {}
/*
** --------------------------------- ACCESSOR ---------------------------------
*/
int Client::getFd() const { return this->_fd; }
const struct sockaddr_in& Client::getAddr() const { return this->_host; }
bool Client::isAlive() const { return this->_alive; }

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
** --------------------------------- METHODS ----------------------------------
*/
bool Client::match(const std::string& input) const {
    return wildcard_match(this->toString().c_str(), input.c_str());
}

void Client::disconnect() {
	this->disconnect("You have been disconnected");
}

void Client::disconnect(const char* reason) {
	if (!this->_alive)
        return;
	std::string res = buildQuitMessage(SERVER_NAME, nickname.c_str(), reason);
	send(_fd, res.c_str(), res.size(), 0);
	close(_fd);
    _alive = false;
    _fd = -1;
}

std::string Client::toString() const {
	/**
	* nickname ≤ 64
	* username ≤ 64
	* host ≤ 128
	**/
	std::stringstream stream;

	stream << this->nickname
		<< '!'
		<< this->name
		<< '@'
		<< inet_ntoa(this->_host.sin_addr);
	return stream.str();
}

void Client::appendBuffer(const std::string& data) {
	_buffer += data;
}
bool Client::hasFullLine() const {
	return _buffer.find("\r\n") != std::string::npos;
}
std::string Client::extractLine() {
	size_t pos = _buffer.find("\r\n");
	if (pos == std::string::npos)
		return "";
	std::string line = _buffer.substr(0, pos);
	_buffer.erase(0, pos + 2);
	return line;
}
/* ************************************************************************** */

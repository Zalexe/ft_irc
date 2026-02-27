#include "Client.hpp"
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
Client::Client() : _fd(-1), _alive(false) {}
Client::Client(int fd, struct sockaddr_in host) : _fd(fd), _host(host), _alive(true) {}
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

}

void Client::disconnect() {
	this->disconnect("You have been disconnected");
}

void Client::disconnect(const char* reason) {
	// TODO
	if (!this->_alive)
        return;

    std::string message = "ERROR :";
    message += reason;
    message += "\r\n";

    send(this->_fd, message.c_str(), message.size(), 0);

    close(this->_fd);
    this->_alive = false;
    this->_fd = -1;
}

std::string Client::toString() const {
	std::stringstream stream;

	stream << this->nickname
		<< '!'
		<< this->name
		<< '@'
		<< inet_ntoa(this->_host.sin_addr);
	return stream.str();
}
/* ************************************************************************** */

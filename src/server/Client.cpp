#include "Client.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sstream>

Client::Client() : fd(-1), alive(false) {}
Client::Client(int fd, struct sockaddr_in host) : fd(fd), host(host), alive(true) {}

Client::~Client() {}

int Client::getFd() const { return this->fd; }
const struct sockaddr_in& Client::getAddr() const { return this->host; }
bool Client::isAlive() const { return this->alive; }

bool Client::match(const std::string& input) const {

}

void Client::disconnect() {
	this->disconnect("You have been disconnected");
}

void Client::disconnect(const char* reason) {
	// TODO
	    if (!this->alive)
        return;

    std::string message = "ERROR :";
    message += reason;
    message += "\r\n";

    send(this->fd, message.c_str(), message.size(), 0);

    close(this->fd);
    this->alive = false;
    this->fd = -1;
}

std::string Client::toString() const {
	std::stringstream stream;

	stream << this->nickname
		<< '!'
		<< this->name
		<< '@'
		<< inet_ntoa(this->host.sin_addr);
	return stream.str();
}

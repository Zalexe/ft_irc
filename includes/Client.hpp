#pragma once

#include <netinet/in.h>
#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sstream>
#include <unistd.h>
#include "Utils.hpp"

class Client {
private:
	int _fd;
	struct sockaddr_in _host;
	std::string _buffer;
	bool _alive;
	bool _authorized;
	bool _registered;
public:
	std::string nickname;
	std::string name;

	Client();
	Client(int fd, struct sockaddr_in host);
	~Client();

	int getFd() const;
	const struct sockaddr_in& getAddr() const;
	bool isAlive() const;

	/**
	* For matching against ban entries.
	* nick?name*!user*@host
	*/
	bool match(const std::string& input) const;
	void disconnect();
	void disconnect(const char* reason);
	std::string toString() const;



	bool isAuthorized() const;
	void setAuthorized(bool);

	bool isRegistered() const;
	void setRegistered(bool);

	void setNick(const std::string&);
	void setUser(const std::string&);

	std::string getNick() const;
	std::string getUser() const;

	void appendBuffer(const std::string& data);
	bool hasFullLine() const;
	std::string extractLine();
};

#pragma once

#include <stdint.h>
#include <netinet/in.h>
#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define USERMODE_INVISIBLE 1 << 0
#define USERMODE_WALLOPS 1 << 1
#define USERMODE_NOTICES 1 << 2

class Client {
private:
	int _fd;
	struct sockaddr_in _host;
	std::string _buffer;
	bool _alive;
	bool _authorized;
	bool _registered;
	uint8_t _modes;
public:
	static const char* NON_OP_MODES;
	static const char* OP_MODES;

	std::string nickname;
	std::string name;
	std::string realName;

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

	uint8_t getUserModes() const;
	bool isMode(uint8_t mode) const;
	void setUserMode(uint8_t mode);
	void unsetUserMode(uint8_t mode);

	bool isAuthorized() const;
	void setAuthorized(bool);

	bool isRegistered() const;
	void setRegistered(bool);

	void setNick(const std::string&);
	void setUser(const std::string&);

	std::string getNick() const;
	std::string getUser() const;

	bool hasNick() const;
	bool hasUser() const;

	void appendBuffer(const std::string& data);
	bool hasFullLine() const;
	std::string extractLine();
};

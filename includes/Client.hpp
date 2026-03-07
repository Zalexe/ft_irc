#pragma once

#include <stdint.h>
#include <netinet/in.h>
#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

class Client {
private:
	int _fd;
	struct sockaddr_in _host;
	std::string _buffer;
	bool _alive;
	bool _authorized;
	bool _registered;

	std::set<Channel*> _channels;
public:
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

	std::string getAvailableModes() const;

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

	void addChannel(Channel* ch);
    void removeChannel(Channel* ch);
    const std::set<Channel*>& getChannels() const;

	void appendBuffer(const std::string& data);
	bool hasFullLine() const;
	std::string extractLine();
};

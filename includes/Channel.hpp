#pragma once

#include <stdint.h>
#include <string>
#include <sys/types.h>
#include <vector>
#include "Client.hpp"

class Channel {
private:
	std::string name;
	std::string topic;
	std::string password;
	const std::vector<Client*> clientList;
	uint16_t maxUsers;
	bool outsideMessages;
	bool moderated;
	bool topicRestricted;

public:
	Channel();
	Channel(const std::string& name);
	Channel(const std::string& name, const std::string& topic);
	Channel(const std::string& name, const std::string& topic, const std::string& passwd);
	~Channel();

	
};

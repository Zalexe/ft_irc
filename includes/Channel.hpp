#pragma once

#include <stdint.h>
#include <string>
#include <sys/types.h>
#include <vector>
#include "Client.hpp"

class Channel {
private:
    std::string _name;
    std::string _topic;
    std::string _key;

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

	// membership
    void addMember(Client*);
    void removeMember(Client*);
    bool isMember(Client*) const;

    // operators
    void addOperator(Client*);
    void removeOperator(Client*);
    bool isOperator(Client*) const;

    // broadcast
    void broadcast(const std::string& msg, Client* exclude = NULL);
};

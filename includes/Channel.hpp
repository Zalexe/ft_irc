#pragma once

#include <stdint.h>
#include <string>
#include <sys/types.h>
#include <set>
#include <vector>
#include "Client.hpp"

class Channel {
private:
    std::string _name;
    std::string _topic;
    std::string _key;

    bool _inviteOnly;        // +i
    bool _topicRestricted;   // +t
    size_t _userLimit;       // +l (0 = unlimited)

    std::set<Client*> _members;
    std::set<Client*> _operators;
    std::set<Client*> _invited;

public:
    Channel(const std::string& name);
    ~Channel();

    // Basic info
    const std::string& getName() const;
    const std::string& getTopic() const;
    void setTopic(const std::string& topic);

    // Membership
    void addMember(Client*);
    void removeMember(Client*);
    bool isMember(Client*) const;

    size_t memberCount() const;

    // Operators
    void addOperator(Client*);
    void removeOperator(Client*);
    bool isOperator(Client*) const;

    // Invite system
    void invite(Client*);
    bool isInvited(Client*) const;

    // Mode handling
    void setInviteOnly(bool);
    bool isInviteOnly() const;

    void setTopicRestricted(bool);
    bool isTopicRestricted() const;

    void setKey(const std::string& key);
    void removeKey();
    bool checkKey(const std::string& key) const;
    bool hasKey() const;

    void setUserLimit(size_t limit);
    void removeUserLimit();
    bool isFull() const;

    // Messaging
    void broadcast(const std::string& msg, Client* exclude = NULL);
};

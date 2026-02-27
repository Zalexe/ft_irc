#include "Channel.hpp"
/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
Channel::Channel(const std::string& name)
    : _name(name),
      _topic(""),
      _key(""),
      _inviteOnly(false),
      _topicRestricted(false),
      _userLimit(0)
{
}
/*
** -------------------------------- DESTRUCTOR --------------------------------
*/
Channel::~Channel(){}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
void Channel::addMember(Client* client)
{
    if (!client)
        return;

    _members.insert(client);
    _invited.erase(client);
    if (_members.size() == 1)
        _operators.insert(client);
}
void Channel::removeMember(Client* client)
{
    if (!client)
        return;

    _members.erase(client);
    _operators.erase(client);
}
bool Channel::isMember(Client* client) const
{
    return _members.find(client) != _members.end();
}

void Channel::addOperator(Client* client)
{
    if (!client)
        return;

    if (isMember(client))
        _operators.insert(client);
}
void Channel::removeOperator(Client* client)
{
    if (!client)
        return;

    _operators.erase(client);
}
bool Channel::isOperator(Client* client) const
{
    return _operators.find(client) != _operators.end();
}
size_t Channel::memberCount() const
{
    return _members.size();
}
bool Channel::isFull() const
{
    if (_userLimit == 0)
        return false;
    return _members.size() >= _userLimit;
}

void Channel::setTopic(const std::string& topic)
{
    _topic = topic;
}
const std::string& Channel::getTopic() const
{
    return _topic;
}
bool Channel::isInvited(Client* client) const
{
    return _invited.find(client) != _invited.end();
}
void Channel::setInviteOnly(bool value)
{
    _inviteOnly = value;
}
bool Channel::isInviteOnly() const
{
    return _inviteOnly;
}
void Channel::setTopicRestricted(bool value)
{
    _topicRestricted = value;
}
bool Channel::isTopicRestricted() const
{
    return _topicRestricted;
}
void Channel::setKey(const std::string& key)
{
    _key = key;
}
void Channel::removeKey()
{
    _key.clear();
}
bool Channel::hasKey() const
{
    return !_key.empty();
}

bool Channel::checkKey(const std::string& key) const
{
    return _key == key;
}
void Channel::setUserLimit(size_t limit)
{
    _userLimit = limit;
}
void Channel::removeUserLimit()
{
    _userLimit = 0;
}
/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
** --------------------------------- METHODS ----------------------------------
*/
bool Channel::isFull() const
{
    if (_userLimit == 0)
        return false;

    return _members.size() >= _userLimit;
}


void Channel::broadcast(const std::string& msg, Client* exclude)
{
    for (std::set<Client*>::iterator it = _members.begin();
         it != _members.end(); ++it)
    {
        if (*it != exclude)
            send((*it)->getFd(), msg.c_str(), msg.size(), 0);
    }
}
/* ************************************************************************** */
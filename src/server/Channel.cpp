#include "Channel.hpp"

/*
** ------------------------------- STATIC --------------------------------
*/
const char* Channel::MODES = "itkol";
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
    client->addChannel(this);
}
bool Channel::removeMember(Client* client)
{
    if (!client)
        return;

    // Remove operator status if applicable
    if(this->isOperator(client))
        _operators.erase(client);
    _members.erase(client);
    client->removeChannel(this);
    if (_operators.empty() && !_members.empty())
    {
        Client* newOp = *_members.begin();
        _operators.insert(newOp);
        std::string opMsg = buildMessage(
            1,
            newOp->getNick().c_str(),
            "MODE",
            (_name + " +o " + newOp->getNick()).c_str()
        );
        broadcast(opMsg);
    }
    return _members.empty();
}
bool Channel::isMember(Client* client) const
{
    return _members.find(client) != _members.end();
}
const std::set<Client*>& Channel::getMembers() const
{
    return _members;
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

const std::string& Channel::getKey() const {
	return _key;
}

size_t Channel::getUserLimit() const {
	return _userLimit;
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
void Channel::broadcast(const std::string& msg, Client* exclude)
{
    const char* data = msg.c_str();
    size_t len = msg.size();

    for (std::set<Client*>::iterator it = _members.begin(); it != _members.end(); ++it)
    {
        if (*it == exclude)
            continue;

        send((*it)->getFd(), data, len, 0);
    }
}
/* ************************************************************************** */

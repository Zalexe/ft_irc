#include "Server.hpp"
#include <Messages.hpp>

void Server::sendNamesList(Client* client, Channel* ch)
{
    std::string names;
    const std::set<Client*>& members = ch->getMembers();
    for (std::set<Client*>::const_iterator it = members.begin(); it != members.end(); ++it)
    {
        if (ch->isOperator(*it))
            names += "@";
        names += (*it)->getNick();
        names += " ";
    }
    std::string namesReply = buildResponseCodeMessage(4,SERVER_NAME,RPL_NAMREPLY,client->getNick().c_str(),"=",ch->getName().c_str(),names.c_str());
    sendMessage(client, namesReply);
    std::string endNames = buildResponseCodeMessage(3,SERVER_NAME,RPL_ENDOFNAMES,client->getNick().c_str(),ch->getName().c_str(),"End of /NAMES list");
    sendMessage(client, endNames);
}


void Server::handleJoin(Client* client, std::stringstream& params)
{
    std::string channelsStr, keysStr;
    if (!(params >> channelsStr))
    {
        sendMessage(client, "Need more parameters for JOIN");
        return;
    }
    std::getline(params, keysStr);
    if (!keysStr.empty() && keysStr[0] == ' ')
        keysStr.erase(0, 1);
    std::vector<std::string> channelNames;
    std::vector<std::string> channelKeys;
    // Split channels
    std::stringstream chStream(channelsStr);
    std::string name;
    while (std::getline(chStream, name, ','))
        channelNames.push_back(name);
    // Split keys
    std::stringstream keyStream(keysStr);
    std::string key;
    while (std::getline(keyStream, key, ','))
        channelKeys.push_back(key);
    for (size_t i = 0; i < channelNames.size(); ++i)
    {
        std::string& chName = channelNames[i];
        std::string key = (i < channelKeys.size()) ? channelKeys[i] : "";
        Channel* ch = getChannelByName(chName);
        if (!ch)
        {
            // New channel: create and make first member operator
            ch = new Channel(chName);
            _channels.push_back(ch);
            ch->addMember(client);
            ch->addOperator(client);
        }
        else
        {
            if (ch->isInviteOnly() && !ch->isInvited(client))
            {
                std::string msg = buildResponseCodeMessage(
                    3, 
                    SERVER_NAME,
                    ERR_INVITEONLYCHAN,
                    client->getNick().c_str(),
                    chName.c_str(),
                    "Cannot join channel (+i)"
                );
                send(client->getFd(), msg.c_str(), msg.size(), 0);
                continue;
            }
            if (ch->hasKey() && ch->checkKey(key) == false)
            {
                std::string msg = buildResponseCodeMessage(
                    3,
                    SERVER_NAME,
                    ERR_BADCHANNELKEY,
                    client->getNick().c_str(),
                    chName.c_str(),
                    "Cannot join channel (+k) - bad key"
                );
                send(client->getFd(), msg.c_str(), msg.size(), 0);
                continue;
            }
            if (ch->isFull())
            {
                std::string msg = buildResponseCodeMessage(
                    3,
                    SERVER_NAME,
                    ERR_CHANNELISFULL,
                    client->getNick().c_str(),
                    chName.c_str(),
                    "Cannot join channel - channel full"
                );
                send(client->getFd(), msg.c_str(), msg.size(), 0);
                continue;
            }
            ch->addMember(client);
        }
        // Broadcast JOIN to other members
        std::string joinMsg = buildMessage(1, client->getNick().c_str(), "JOIN", chName.c_str());
        send(client->getFd(), joinMsg.c_str(), joinMsg.size(), 0);
        ch->broadcast(joinMsg, client);
        // Send topic if any
        if (!ch->getTopic().empty())
        {
            std::string topicMsg = buildMessage(1, SERVER_NAME, TOPIC, client->getNick().c_str(), chName.c_str(), ch->getTopic().c_str());
            send(client->getFd(), topicMsg.c_str(), topicMsg.size(), 0);
        }
        sendNamesList(client, ch);
        // Send current channel modes
        std::string modeMsg = buildResponseChannelModeIs(*client, *ch);
        send(client->getFd(), modeMsg.c_str(), modeMsg.size(), 0);
        // Remove client from invite list if present
        if (ch->isInvited(client))
            ch->removeInvite(client);
    }
}

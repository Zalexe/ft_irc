#include "Server.hpp"
#include <Messages.hpp>

void Server::handleTopic(Client* client, std::stringstream& params)
{
    std::string channelName;
    if (!(params >> channelName))
    {
        sendMessage(client, "No channel specified for TOPIC");
        return;
    }
    Channel* ch = getChannelByName(channelName);
    if (!ch)
    {
        std::string msg = buildResponseNoSuchChannel(client->getNick().c_str(), channelName.c_str());
        send(client->getFd(), msg.c_str(), msg.size(), 0);
        return;
    }
    if (!ch->isMember(client))
    {
        std::string msg = buildResponseUserNotInChannel(client->getNick().c_str(), client->getNick().c_str(), channelName.c_str());
        send(client->getFd(), msg.c_str(), msg.size(), 0);
        return;
    }
    std::string topic;
    std::getline(params, topic);
    if (!topic.empty() && topic[0] == ' ')
        topic.erase(0, 1);  // trim leading space
    if (!topic.empty() && topic[0] == ':')
        topic.erase(0, 1);  // remove leading colon
    if (topic.empty())
    {
        if (!ch->getTopic().empty())
        {
            std::string msg = buildResponseCodeMessage(3, SERVER_NAME, TOPIC,client->getNick().c_str(),ch->getName().c_str(),ch->getTopic().c_str()
            );
            send(client->getFd(), msg.c_str(), msg.size(), 0);
        }
        else
        {
            std::string msg = buildResponseCodeMessage(2, SERVER_NAME, NO_TOPIC,client->getNick().c_str(),ch->getName().c_str());
            send(client->getFd(), msg.c_str(), msg.size(), 0);
        }
        return;
    }
    if (ch->isTopicRestricted() && !ch->isOperator(client))
    {
        std::string msg = buildResponseChannelOpNeeded(client->getNick().c_str(), ch->getName().c_str());
        send(client->getFd(), msg.c_str(), msg.size(), 0);
        return;
    }
    ch->setTopic(topic);
    std::string topicMsg = buildMessage(2,client->getNick().c_str(),"TOPIC",ch->getName().c_str(),topic.c_str());
    ch->broadcast(topicMsg);
}

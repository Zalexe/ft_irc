#include "Server.hpp"
#include <Messages.hpp>

void Server::handleKick(Client* client, std::stringstream& params)
{
    std::string channelName, targetNick, reason;
    if (!(params >> channelName >> targetNick))
    {
        sendError(client, "Need more parameters for KICK");
        return;
    }
    std::getline(params, reason);
    if (!reason.empty() && reason[0] == ' ')
        reason.erase(0, 1);
    Channel* ch = getChannelByName(channelName);
    if (!ch)
    {
        std::string msg = buildResponseCodeMessage(3, SERVER_NAME, NO_SUCH_CHANNEL, client->getNick().c_str(), channelName.c_str(), "No such channel");
        send(client->getFd(), msg.c_str(), msg.size(), 0);
        return;
    }
    
    Client* target = getClientByName(targetNick);
    if (!target || !ch->isMember(target))
    {
        std::string msg = buildResponseCodeMessage(4, SERVER_NAME, ERR_USERNOTINCHANNEL, client->getNick().c_str(), targetNick.c_str(), channelName.c_str(), "They aren't on that channel");
        send(client->getFd(), msg.c_str(), msg.size(), 0);
        return;
    }
    if (!ch->isOperator(client))
    {
        std::string msg = buildResponseCodeMessage(3, SERVER_NAME, ERR_CHANOPRIVSNEEDED, client->getNick().c_str(), channelName.c_str(), "You're not channel operator");
        send(client->getFd(), msg.c_str(), msg.size(), 0);
        return;
    }
    std::string kickMsg = buildMessage(3, client->getNick().c_str(), "KICK", channelName.c_str(), targetNick.c_str(), reason.c_str());
    ch->broadcast(kickMsg);
    bool empty = ch->removeMember(target);
    if (empty)
    {
        _channels.erase(std::remove(_channels.begin(), _channels.end(), ch), _channels.end());
        delete ch;
    }
}


#include "Server.hpp"
#include <Messages.hpp>

void Server::handleKick(Client* client, std::stringstream& params)
{
    std::string channelName, targetNick, reason;
    if (!(params >> channelName >> targetNick))
    {
        sendMessage(client, buildResponseNeedMoreParams(client->nickname.c_str(), "KICK"));
        return;
    }
    std::getline(params, reason);
    if (!reason.empty() && reason[0] == ' ')
        reason.erase(0, 1);
    Channel* ch = getChannelByName(channelName);
    if (!ch)
    {
        std::string msg = buildResponseCodeMessage(3, NO_SUCH_CHANNEL, client->getNick().c_str(), channelName.c_str(), "No such channel");
		sendMessage(client, msg);
        return;
    }
    
    Client* target = getClientByName(targetNick);
    if (!target || !ch->isMember(target))
    {
        std::string msg = buildResponseCodeMessage(4, ERR_USERNOTINCHANNEL, client->getNick().c_str(), targetNick.c_str(), channelName.c_str(), "They aren't on that channel");
		sendMessage(client, msg);
        return;
    }
    if (!ch->isOperator(client))
    {
        std::string msg = buildResponseCodeMessage(3, ERR_CHANOPRIVSNEEDED, client->getNick().c_str(), channelName.c_str(), "You're not channel operator");
		sendMessage(client, msg);
        return;
    }
    std::string kickMsg = buildMessage(3, client->getNick().c_str(), "KICK", channelName.c_str(), targetNick.c_str(), reason.c_str());
    ch->broadcast(kickMsg);
    bool empty = ch->removeMember(target);
    target->removeChannel(ch);
    if (empty)
    {
        _channels.erase(std::remove(_channels.begin(), _channels.end(), ch), _channels.end());
        delete ch;
    }
}


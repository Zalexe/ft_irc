#include "Server.hpp"
#include <Messages.hpp>

void Server::handlePrivmsg(Client* client, std::stringstream& params)
{
    std::string targetName;
    if (!(params >> targetName))
    {
        sendMessage(client, buildResponseNeedMoreParams(client->nickname.c_str(), "PRIVMSG"));
        return;
    }
    std::string message;
    std::getline(params, message);
    if (!message.empty() && message[0] == ' ')
        message.erase(0, 1); // remove leading space
    if (message.empty() || message[0] != ':')
    {
        sendMessage(client, buildResponseCodeMessage(2, ERR_NOTEXTTOSEND, client->getNick().c_str(), "No text to send"));
        return;
    }
    message.erase(0, 1);
    if (targetName[0] == '#') //target is a channel
    {
        Channel* ch = getChannelByName(targetName);
        if (!ch)
        {
            std::string msg = buildResponseNoSuchChannel(client->getNick().c_str(), targetName.c_str());
			sendMessage(client, msg);
            return;
        } else if (!ch->isMember(client))
        {
            std::string msg = buildResponseNotOnChannel(client->getNick().c_str(), ch->getName().c_str());
			sendMessage(client, msg);
            return;
        }
        std::string privMsg = buildMessage(2, client->getNick().c_str(), "PRIVMSG", ch->getName().c_str(), message.c_str());
        ch->broadcast(privMsg, client);
    }
    else
    {
        Client* target = getClientByName(targetName);
        if (!target)
        {
            std::string msg = buildResponseCodeMessage(3, NO_SUCH_NICKNAME, client->getNick().c_str(), targetName.c_str(), "No such nick");
			sendMessage(client, msg);
            return;
        }
        std::string privMsg = buildMessage(2, client->getNick().c_str(), "PRIVMSG", target->getNick().c_str(), message.c_str());
		sendMessage(target, privMsg);
    }
}

#include "Server.hpp"
#include <Messages.hpp>

void Server::handlePrivmsg(Client* client, std::stringstream& params)
{
    std::string targetName;
    if (!(params >> targetName))
    {
        sendError(client, "No target for PRIVMSG");
        return;
    }
    std::string message;
    std::getline(params, message);
    if (!message.empty() && message[0] == ' ')
        message.erase(0, 1); // remove leading space
    if (message.empty() || message[0] != ':')
    {
        sendError(client, "No text to send");
        return;
    }
    message.erase(0, 1);
    if (targetName[0] == '#') //target is a channel
    {
        Channel* ch = getChannelByName(targetName);
        if (!ch || !ch->isMember(client))
        {
            std::string msg = buildResponseCodeMessage(3, SERVER_NAME, ERR_CANNOTSENDTOCHAN, client->getNick().c_str(), targetName.c_str(), "Cannot send to channel");
            send(client->getFd(), msg.c_str(), msg.size(), 0);
            return;
        }
        std::string privMsg = buildMessage(2, client->getNick().c_str(), "PRIVMSG", targetName.c_str(), message.c_str());
        ch->broadcast(privMsg, client);
    }
    else
    {
        Client* target = getClientByName(targetName);
        if (!target)
        {
            std::string msg = buildResponseCodeMessage(3, SERVER_NAME, NO_SUCH_NICKNAME, client->getNick().c_str(), targetName.c_str(), "No such nick");
            send(client->getFd(), msg.c_str(), msg.size(), 0);
            return;
        }
        std::string privMsg = buildMessage(2, client->getNick().c_str(), "PRIVMSG", targetName.c_str(), message.c_str());
        send(target->getFd(), privMsg.c_str(), privMsg.size(), 0);
    }
}

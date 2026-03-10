#include "Server.hpp"
#include <Messages.hpp>

void Server::handlePrivmsg(Client* client, std::stringstream& params)
{
    std::string targetName;
    if (!(params >> targetName))
    {
        sendMessage(client, "No target for PRIVMSG");
        return;
    }
    std::string message;
    std::getline(params, message);
    if (!message.empty() && message[0] == ' ')
        message.erase(0, 1); // remove leading space
    if (message.empty() || message[0] != ':')
    {
        sendMessage(client, "No text to send");
        return;
    }
    message.erase(0, 1);
    if (targetName[0] == '#') //target is a channel
    {
        Channel* ch = getChannelByName(targetName);
        if (!ch || !ch->isMember(client))
        {
            std::string msg = buildResponseCodeMessage(3, SERVER_NAME, ERR_CANNOTSENDTOCHAN, client->getNick().c_str(), targetName.c_str(), "Cannot send to channel");
			sendMessage(client, msg);
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
			sendMessage(client, msg);
            return;
        }
        std::string privMsg = buildMessage(2, client->getNick().c_str(), "PRIVMSG", targetName.c_str(), message.c_str());
		sendMessage(client, privMsg);
    }
}

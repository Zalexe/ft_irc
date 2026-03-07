#include "Server.hpp"


void Server::handleQuit(Client* client, const std::string& reason)
{
    std::string quitReason = reason.empty()
        ? client->getNick() + " has quit"
        : reason;

    std::string quitMsg = buildMessage(
        1,
        client->getNick().c_str(),
        "QUIT",
        quitReason.c_str()
    );

    for (Channel* ch : client->getChannels())
    {
        ch->broadcast(quitMsg, client);
        ch->removeMember(client);
    }

    disconnectClient(client->getFd());
}

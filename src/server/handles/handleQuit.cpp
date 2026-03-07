#include "Server.hpp"


void Server::broadcastToSharedChannels(Client* client, const std::string& msg)
{
    std::set<Client*> notified;

    for (size_t i = 0; i < _channels.size(); ++i)
    {
        Channel* channel = _channels[i];

        if (!channel->isMember(client))
            continue;

        const std::set<Client*>& members = channel->getMembers();

        for (std::set<Client*>::iterator it = members.begin(); it != members.end(); ++it)
        {
            Client* target = *it;

            if (target == client)
                continue;

            if (notified.insert(target).second)
                send(target->getFd(), msg.c_str(), msg.size(), 0);
        }
    }
}

void Server::handleQuit(int fd, const std::string& reason)
{
    Client* client = getClientByFd(fd);
    if (!client)
        return;

    std::string quitReason = reason.empty()
        ? client->getNick() + " has quit"
        : reason;

    std::string quitMsg = buildMessage(
        1,
        client->getNick().c_str(),
        "QUIT",
        quitReason.c_str()
    );

    broadcastToSharedChannels(client, quitMsg);

    disconnectClient(fd);
}

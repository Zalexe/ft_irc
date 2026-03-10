#include "Server.hpp"
#include <Messages.hpp>

void Server::handleQuit(Client* client, const std::string& reason)
{
    if (!client)
        return;
    std::string quitReason = reason.empty() ? client->getNick() + " has quit" : reason;
    std::string quitMsg = buildMessage(1, client->getNick().c_str(), "QUIT", quitReason.c_str());
    for (size_t i = 0; i < _channels.size();)
    {
        Channel* ch = _channels[i];

        if (ch->isMember(client))
        {
            ch->broadcast(quitMsg, client);
            bool empty = ch->removeMember(client);
            if (empty)
            {
                delete ch;
                _channels.erase(_channels.begin() + i);
                continue; // do not increment i
            }
        }
        ++i;
    }
    client->disconnect(quitReason.c_str());
    int fd = client->getFd();
    epoll_ctl(_epollSocket, EPOLL_CTL_DEL, fd, NULL);
    close(fd);
    _clients.erase(std::remove(_clients.begin(), _clients.end(), client), _clients.end());
    delete client;
}

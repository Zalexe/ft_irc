#include "Server.hpp"



void Server::handleQuit(Client& client, const std::string& reason)
{
    std::string quitMsg = ":" + client.getPrefix() +
                          " QUIT :" + reason + "\r\n";

    broadcastToSharedChannels(client, quitMsg);

    client.disconnect(reason);
}

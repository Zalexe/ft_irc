#include "Server.hpp"
#include "Utils.hpp"
#include <Messages.hpp>

void Server::handleWho(Client* client, std::stringstream& params)
{
    std::string channelsStr;
    if (!(params >> channelsStr))
    {
        sendMessage(client, buildResponseNeedMoreParams(client->nickname.c_str(), "WHO"));
        return;
    }
    if(channelsStr[0] != '#')
    {
        sendMessage(client, buildResponseNoSuchChannel(client->getNick().c_str(), channelsStr.c_str()));
        return;
    }
    std::stringstream chStream(channelsStr);
    std::string name;
    std::getline(chStream, name, ',');
	Channel* ch = getChannelByName(name);
	std::cout << name << std::endl;
	if(!ch->isMember(client))
	{
		sendMessage(client, buildResponseNotOnChannel(client->nickname.c_str(), ch->getName().c_str()));
		return;
	}
	//Send names of users in channels
	sendNamesList(client, ch);
}

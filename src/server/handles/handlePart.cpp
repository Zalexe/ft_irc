#include "Server.hpp"
#include <Messages.hpp>

void Server::handlePart(Client* client, std::stringstream& params)
{
    std::string channelsStr;
    if (!(params >> channelsStr))
    {
        sendMessage(client, buildResponseNeedMoreParams(client->nickname.c_str(), "PART"));
        return;
    }
    if(channelsStr[0] != '#')
    {
        sendMessage(client, buildResponseNoSuchChannel(client->getNick().c_str(), channelsStr.c_str()));
        return;
    }
    std::vector<std::string> channelNames;
    // Split channels
    std::stringstream chStream(channelsStr);
    std::string name;
    while (std::getline(chStream, name, ','))
        channelNames.push_back(name);
    // Split keys
    std::string reason;
	params >> reason;
	if (!reason.empty()) {
		if (reason[0] == ':')
			reason.erase(0, 1);
		else reason = "";
	}
    for (size_t i = 0; i < channelNames.size(); ++i)
    {
        std::string& chName = channelNames[i];
        Channel* ch = getChannelByName(chName);
        if (!ch)
        {
			sendMessage(client, buildResponseNoSuchChannel(client->getNick().c_str(), channelsStr.c_str()));
			return;
        }
        else
        {
            if(!ch->isMember(client))
            {
				sendMessage(client, buildResponseNotOnChannel(client->nickname.c_str(), ch->getName().c_str()));
                continue;
            }
			//Broadcast PART to other members
			std::string partMsg;
			if (reason.empty())
				partMsg = buildMessage(1, client->getNick().c_str(), "PART", chName.c_str());
			else
				partMsg = buildMessage(1, client->getNick().c_str(), "PART", chName.c_str(), reason.c_str());
			ch->broadcast(partMsg);
			ch->removeMember(client);
		}
	}
}

#pragma once

#include "Channel.hpp"
#include <string>

class Bot {
private:
	std::string nick;
	std::string processMsg(const std::string& input) const;
public:
	Bot();
	Bot(const std::string name);
	Bot(const Bot& other);
	~Bot();

	void onChannelPrivmsg(const Channel& channel, const std::string msg) const;

	Bot& operator=(const Bot& other);
};

#pragma once

class Client;
#include <string>
#include <cstring>

std::string quitMessage(char* reason);

bool validatePassword(const char* str, std::string &password);
bool validatePort(const char* str, int &port);

//int CheckArgs(char** argv);
bool wildcard_match(const char* str, const char* pattern);

std::string toLower(const std::string& str);

void sendMessage(Client* client, const std::string& msg);

#pragma once

// Registration
#include "Client.hpp"
#define WELCOME "001"
#define YOUR_HOST "002"
#define SERVER_CREATED "003"
#define SERVER_VERSION "004"

// Errors
#define NO_SUCH_NICKNAME "401"
#define NO_SUCH_CHANNEL "403"
#define NICKNAME_IN_USE "433"
#define NOT_REGISTERED "451"

// Whois
#define WHOISUSER "311"
#define WHOISSERVER "312"
#define WHOISOPERATOR "313"
#define ENDOFWHOIS "318"

// Channel info
#define NO_TOPIC "331"
#define TOPIC "332"
#define TOPIC_METADATA "333"
#define NAMES_LIST "353"
#define END_OF_NAMES "366"

#include <string>

/**
* Builds a regular command message with variable params.
* 'n' is the amount of params after 'cmd'.
* it prefixes the last param with ':' automatically
* The message shall not be more than 512 characters long, a runtime error is thrown otherwise.
*/
std::string buildMessage(int n, const char* sender, const char* cmd, ...);
std::string buildQuitMessage(const char* sender, const char* targetNick, const char* reason);
std::string buildResponseCodeMessage(int n, const char* code, ...);

// Registration (these 4 messages are sent)
std::string buildResponseWelcome(const Client* target);
std::string buildResponseYourhost(const Client* target);
std::string buildResponseCreationDate(const Client* target);
std::string buildResponseMyInfo(const Client* target);

// Info
std::string buildResponseWhoisuser(const char* targetNick, const Client* user);

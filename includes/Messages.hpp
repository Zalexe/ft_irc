#pragma once

#include "Client.hpp"
#include "Server.hpp"

// Registration
#define WELCOME "001"
#define YOUR_HOST "002"
#define SERVER_CREATED "003"
#define SERVER_MYINFO "004"

// Errors
#define NO_SUCH_NICKNAME "401"
#define NO_SUCH_CHANNEL "403"
#define UNKNOWN_ERROR "421"
#define NO_NICKNAME_GIVEN "431"
#define NICKNAME_IN_USE "433"
#define NOT_REGISTERED "451"
#define NOT_ENOUGH_PARAM "461"
#define ALREADY_REG "462"
#define INCORRECT_PASS "464"

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

// Errors
#define NOPRIVILEGES "481"

#include <string>

/**
 * Builds a regular command message with variable params.
 * 'n' is the amount of params after 'cmd'.
 * 'sender' may be a user's host or SERVER_NAME, could be whatever really
 * cmd is the command to execute, maybe a 3-digit code or something like "PRIVMSG" or "QUIT"
 * it prefixes the last param with ':' automatically
 */
std::string buildMessage(int n, const char* sender, const char* cmd, ...);
std::string buildQuitMessage(const char* sender, const char* targetNick, const char* reason);
std::string buildResponseCodeMessage(int n, const char* code, ...);

// Registration (these 4 messages are sent)
std::string buildResponseWelcome(const Client& target);
std::string buildResponseYourhost(const Client& target);
std::string buildResponseCreationDate(const Client& target);
std::string buildResponseMyInfo(const Client& target);

// Info
std::string buildResponseWhoisuser(const char* targetNick, const Client& user);

// Error
std::string buildResponseNoPrivileges(const char* targetNick);

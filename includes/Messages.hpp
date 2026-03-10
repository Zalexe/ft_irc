#pragma once

#include "Server.hpp"

// Registration
#define WELCOME "001"
#define YOUR_HOST "002"
#define SERVER_CREATED "003"
#define SERVER_MYINFO "004"

// Errors
#define NO_SUCH_NICKNAME "401"
#define NO_SUCH_CHANNEL "403"
#define ERR_CANNOTSENDTOCHAN "404"
#define ERR_NOTEXTTOSEND "412"
#define UNKNOWN_ERROR "421"
#define NO_NICKNAME_GIVEN "431"
#define NICKNAME_IN_USE "433"
#define ERR_USERNOTINCHANNEL "441"
#define ERR_NOTONCHANNEL "442"
#define USERINCHANNEL "443"
#define NOT_REGISTERED "451"
#define NOT_ENOUGH_PARAM "461"
#define ALREADY_REG "462"
#define INCORRECT_PASS "464"
#define ERR_KEYSET "467"
#define ERR_CHANNELISFULL "471"
#define ERR_INVITEONLYCHAN "473"
#define ERR_BADCHANNELKEY "475"
#define ERR_CHANOPRIVSNEEDED "482"

// Whois
#define WHOISUSER "311"
#define WHOISSERVER "312"
#define WHOISOPERATOR "313"
#define ENDOFWHOIS "318"

#define RPL_NAMREPLY "353"
#define RPL_ENDOFNAMES "366"

// Channel info
#define CHANNEL_MODE_IS "324"
#define NO_TOPIC "331"
#define TOPIC "332"
#define TOPIC_METADATA "333"
#define INVITE_SUCCESS "341"
#define INVITE_LIST "346"
#define END_OF_INVITE_LIST "347"
#define NAMES_LIST "353"
#define END_OF_NAMES "366"

// Errors
#define NOSUCHNICK "401"
#define NOSUCHCHANNEL "403"
#define USER_NOT_IN_CHANNEL "441"
#define NEEDMOREPARAMS "461"
#define UNKNOWN_CHANNEL_MODE "472"
#define NOPRIVILEGES "481"
#define CHANNEL_OP_NEEDED "482"

#include <string>

/**
 * Builds a regular command message with variable params.
 * 'n' is the amount of params after 'cmd'.
 * 'sender' may be a user's host or SERVER_NAME, could be whatever really
 * cmd is the command to execute, maybe a 3-digit code or something like "PRIVMSG" or "QUIT"
 * it prefixes the last param with ':' automatically
 */
std::string buildMessage(int n, const char* sender, const char* cmd, ...);
std::string buildMessageNoTrail(int n, const char* sender, const char* cmd, ...);
std::string buildQuitMessage(const char* sender, const char* targetNick, const char* reason);
std::string buildResponseCodeMessage(int n, const char* code, ...);
std::string buildResponseCodeMessageNoTrail(int n, const char* code, ...);

// Registration (these 4 messages are sent)
std::string buildResponseWelcome(const Client& target);
std::string buildResponseYourhost(const Client& target);
std::string buildResponseCreationDate(const Client& target);
std::string buildResponseMyInfo(const Client& target);

// Info
std::string buildResponseWhoisuser(const char* targetNick, const Client& user);
std::string buildResponseInviting(const char* targetNick, const char* channel, const char* invited);
std::string buildResponseInviteListSingle(const char* targetNick, const char* channel, const char* invitemask);
std::string buildResponseEndOfInviteList(const char* targetNick, const char* channel);
std::string buildResponsesInviteList(const char* targetNick, const Channel& channel);
std::string buildResponseChannelModeIs(Client& target, const Channel& channel);

// Error
std::string buildResponseNoPrivileges(const char* targetNick);
std::string buildResponseChannelOpNeeded(const char* targetNick, const char* channel);
std::string buildResponseNoSuchChannel(const char* targetNick, const char* channel);
std::string buildResponseUnknownChannelMode(const char* targetNick, char mode, const char* channel);
std::string buildResponseNeedMoreParams(const char* targetNick, const char* cmd);
std::string buildResponseUserNotInChannel(const char* targetNick, const char* nickNotFound, const char* channel);
std::string buildResponseNoSuchNick(const char* targetNick, const char *nick);
std::string buildResponseUserAlreadyInChannel(const char* targetNick, const char* nick, const char* channel);
std::string buildResponseNotOnChannel(const char* targetNick, const char* channel);

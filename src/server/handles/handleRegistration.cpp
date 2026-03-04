#include "Server.hpp"

/*
** --------------------------------- HANDLE PASS ----------------------------------
*/
void Server::handlePass(Client* client, const std::string& line)
{
    if (client->isRegistered())
    {
        std::string e = buildResponseCodeMessage(2, ALREADY_REG, client->nickname.c_str(), "You may not reregister");
        send(client->getFd(), e.c_str(), e.length(), 0);
        return;
    }

    std::stringstream ss(line);
    std::string cmd, pass;
    ss >> cmd >> pass;

    if (pass.empty())
    {
        std::string e = buildResponseCodeMessage(2, NOT_ENOUGH_PARAM, client->nickname.c_str(), "Not enough parameters");
        send(client->getFd(), e.c_str(), e.length(), 0);
        return;
    }

    if (pass != _pass)
    {
        std::string e = buildResponseCodeMessage(2, INCORRECT_PASS, client->nickname.c_str(), "Password incorrect");
        send(client->getFd(), e.c_str(), e.length(), 0);
        disconnectClient(client->getFd());
        return;
    }

    client->setAuthorized(true);
}
/*
** --------------------------------- HANDLE NICK ----------------------------------
*/
std::string Server::toLower(const std::string& str)
{
    std::string result = str;
    for (size_t i = 0; i < result.size(); ++i)
        result[i] = std::tolower(result[i]);
    return result;
}
bool Server::nickExists(const std::string& nick, Client* requester)
{
    std::string lowerNick = toLower(nick);

    for (size_t i = 0; i < _clients.size(); ++i)
    {
        Client* client = _clients[i];
        if (client == requester)
            continue;
        if (toLower(client->getNick()) == lowerNick)
            return true;
    }
    return false;
}

void Server::handleNick(Client* client, const std::string& line)
{
    std::stringstream ss(line);
    std::string cmd, nick;
    ss >> cmd >> nick;

    if (nick.empty())
    {
        std::string e = buildResponseCodeMessage(1, NO_NICKNAME_GIVEN, "No nickname given");
        send(client->getFd(), e.c_str(), e.length(), 0);
        return;
    }

    if (nickExists(nick, client))
    {
        std::string e = buildResponseCodeMessage(1, NICKNAME_IN_USE, "Nickname in use");
        send(client->getFd(), e.c_str(), e.length(), 0);
        return;
    }

    client->setNick(nick);
}

/*
** --------------------------------- HANDLE USER ----------------------------------
*/
void Server::handleUser(Client* client, const std::string& line)
{
    if (client->isRegistered())
    {
        std::string e = buildResponseCodeMessage(2, ALREADY_REG, client->nickname.c_str(), "You may not reregister");
        send(client->getFd(), e.c_str(), e.length(), 0);
        return;
    }

    std::stringstream ss(line);
    std::string cmd, username, hostname, servername, realname;

    ss >> cmd >> username >> hostname >> servername;

    if (username.empty() || hostname.empty() || servername.empty())
    {
        std::string e = buildResponseCodeMessage(2, NOT_ENOUGH_PARAM, client->nickname.c_str(), "Not enough parameters");
        send(client->getFd(), e.c_str(), e.length(), 0);
        return;
    }
    std::getline(ss, realname);

    if (realname.empty())
    {
        std::string e = buildResponseCodeMessage(2, NOT_ENOUGH_PARAM, client->nickname.c_str(), "Not enough parameters");
        send(client->getFd(), e.c_str(), e.length(), 0);
        return;
    }
    if (realname[0] == ' ')
        realname.erase(0, 1);
    if (!realname.empty() && realname[0] == ':')
        realname.erase(0, 1);
    client->setUser(username);
}

/*
** --------------------------------- HANDLE REGISTRATION ----------------------------------
*/
void Server::sendWelcome(Client* client)
{
    std::string ss = buildResponseWelcome(*client);
    ssize_t sent = send(client->getFd(), ss.c_str(), ss.length(), 0);
    if (sent < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
        }
        else
        {
            disconnectClient(client->getFd());
        }
    }
}

void Server::tryRegister(Client* client)
{
    if (client->isRegistered())
        return;
    if (!client->hasNick())
        return;
    if (!client->hasUser())
        return;
    if (!client->isAuthorized())
        return;

    client->setRegistered(true);
    sendWelcome(client);
}


void Server::handleRegistration(Client* client, const std::string& command, const std::string& line)
{
    if (command == "PASS"){
        handlePass(client, line);
        tryRegister(client);
    }
    else if (command == "NICK"){
        handleNick(client, line);
        tryRegister(client);
    }
    else if (command == "USER"){
        handleUser(client, line);
        tryRegister(client);
    }
    else
    {
        std::string e = buildResponseCodeMessage(2, NOT_REGISTERED, client->nickname.c_str(), "Not registered");
        send(client->getFd(), e.c_str(), e.length(), 0);
    }
    tryRegister(client);
}
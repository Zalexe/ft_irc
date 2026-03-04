#include "Server.hpp"
#include <cstddef>
#include <ctime>


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


void Server::handleRegistration(Client* client,
                                const std::string& command,
                                const std::string& line)
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
        //replace with the message functions
        sendError(client, NOT_REGISTERED);
    }
    tryRegister(client);
}
void Server::handlePass(Client* client, const std::string& line){
    if (client->isRegistered())
    {
        //replace with the message functions
        send(client->getFd(), "462 :You may not reregister\r\n", 31, 0);
        return;
    }

    std::stringstream ss(line);
    std::string cmd, pass;
    ss >> cmd >> pass;

    if (pass.empty())
    {
        //replace with the message functions
        send(client->getFd(), "461 PASS :Not enough parameters\r\n", 36, 0);
        return;
    }

    if (pass != _pass)
    {
        //replace with the message functions
        send(client->getFd(), "464 :Password incorrect\r\n", 27, 0);
        disconnectClient(client->getFd());
        return;
    }

    client->setAuthorized(true);
}

bool Server::nickExists(const std::string& nick){}

void Server::handleNick(Client* client, const std::string& line)
{
    std::stringstream ss(line);
    std::string cmd, nick;
    ss >> cmd >> nick;

    if (nick.empty())
    {
        send(client->getFd(), "431 :No nickname given\r\n", 26, 0);
        return;
    }

    if (nickExists(nick))
    {
        send(client->getFd(),
             "433 * " + nick + " :Nickname is already in use\r\n",
             43 + nick.length(), 0);
        return;
    }

    client->setNick(nick);
}
void Server::handleUser(Client* client, const std::string& line)
{
    if (client->isRegistered())
    {
        send(client->getFd(),
             "462 :You may not reregister\r\n",
             31, 0);
        return;
    }

    std::stringstream ss(line);
    std::string cmd, username, hostname, servername, realname;

    ss >> cmd >> username >> hostname >> servername;

    if (username.empty() || hostname.empty() || servername.empty())
    {
        send(client->getFd(),
             "461 USER :Not enough parameters\r\n",
             36, 0);
        return;
    }

    // Get the rest as realname
    std::getline(ss, realname);

    if (realname.empty())
    {
        send(client->getFd(),
             "461 USER :Not enough parameters\r\n",
             36, 0);
        return;
    }

    // Remove leading space
    if (realname[0] == ' ')
        realname.erase(0, 1);

    // Remove leading ':'
    if (!realname.empty() && realname[0] == ':')
        realname.erase(0, 1);

    client->setUser(username);
}
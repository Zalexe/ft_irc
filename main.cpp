#include <iostream>
#include "Server.hpp"
#include "Utils.hpp"

int main(int argc, char** argv)
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: ./ircserv <PORT> <PASSWORD>\n";
            return 1;
        }
        int port;
        if (!validatePort(argv[1], port))
        {
            std::cerr << "Error: Port must be numeric between 1024 and 65535\n";
            return 1;
        }
        std::string password;
        if (!validatePassword(argv[2], password))
        {
            std::cerr << "Error: Password cannot be empty\n";
            return 1;
        }
        Server server(port, password);
        server.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
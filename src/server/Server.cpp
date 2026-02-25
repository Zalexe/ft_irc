
#include "Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/
Server::Server(){}

Server::Server(int Port, std::string Pass)
    : _pass(Pass), _port(Port)
{
    struct sockaddr_in address;
    int opt = 1;

    //Create socket
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverSocket < 0)
        throw std::runtime_error("socket failed");

    //Reuse address
    if (setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        throw std::runtime_error("setsockopt failed");

    //Set non-blocking
    fcntl(_serverSocket, F_SETFL, O_NONBLOCK);

    //Configure address
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(_port);

    //Bind
    if (bind(_serverSocket, (struct sockaddr*)&address, sizeof(address)) < 0)
        throw std::runtime_error("bind failed");

    //Listen
    if (listen(_serverSocket, SOMAXCONN) < 0)
        throw std::runtime_error("listen failed");

    //Create epoll
    _epollSocket = epoll_create(1);
    if (_epollSocket < 0)
        throw std::runtime_error("epoll_create failed");

    //Add server socket to epoll
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = _serverSocket;

    if (epoll_ctl(_epollSocket, EPOLL_CTL_ADD, _serverSocket, &ev) < 0)
        throw std::runtime_error("epoll_ctl failed");
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

/*
** --------------------------------- METHODS ----------------------------------
*/
void Server::run()
{
    struct epoll_event events[1024];

    while (true)
    {
        int nfds = epoll_wait(_epollSocket, events, 1024, -1);
        if (nfds < 0)
            continue;

        for (int i = 0; i < nfds; i++)
        {
            if (events[i].data.fd == _serverSocket)
            {
                acceptClient();
            }
            else
            {
                handleClient(events[i].data.fd);
            }
        }
    }
}

void Server::acceptClient()
{
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);

    int client_fd = accept(_serverSocket,
                           (struct sockaddr*)&client_addr,
                           &len);

    if (client_fd < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return;
        else
        {
            std::cerr << "accept failed: " << strerror(errno) << std::endl;
            return;
        }
    }
    // Non-blocking
    fcntl(client_fd, F_SETFL, O_NONBLOCK);

    // Add to epoll
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = client_fd;

    epoll_ctl(_epollSocket, EPOLL_CTL_ADD, client_fd, &ev);

    // Create Client object
    Client* newClient = new Client(client_fd);
    _clients.push_back(newClient);
}

void Server::handleClient(int fd)
{
    char buffer[512];
    ssize_t bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return; // nothing to read yet
        else
        {
            disconnectClient(fd);
            return;
        }
    } else if (bytes == 0)
    {
        // client closed connection
        disconnectClient(fd);
        return;
    }

    buffer[bytes] = '\0';

    Client* client = getClientByFd(fd);
    client->appendBuffer(buffer);

    // Process full lines
    while (client->hasFullLine())
    {
        std::string line = client->extractLine();
        processCommand(client, line);
    }
}

void Server::sendWelcome(Client* client)
{
    std::string nick = client->getNick();
    std::string user = client->getUser();
    std::string host = "localhost";

    std::stringstream ss;

    ss << ":" << _serverPrefix
       << " 001 " << nick
       << " :Welcome to the Internet Relay Network "
       << nick << "!" << user << "@" << host << "\r\n";

    ssize_t sent = send(client->getFd(), ss.str().c_str(), ss.str().length(), 0);
    if (sent < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            // optional: buffer for later
        }
        else
        {
            disconnectClient(client->getFd());
        }
    }
}

void Server::processCommand(Client* client, std::string line)
{
    
}
/*
** -------------------------------- DESTRUCTOR --------------------------------
*/
Server::~Server(){}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/
/* ************************************************************************** */

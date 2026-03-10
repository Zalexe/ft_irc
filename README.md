*This project has been created as part of the 42 curriculum by intherna, cmarrued.*

# ft_irc

## Description

**ft_irc** is a simple IRC (Internet Relay Chat) server implemented in C++98.  
The goal of this project is to build a networked application using low-level socket programming, non-blocking I/O, and event-driven design.  

The server handles multiple clients concurrently, allowing them to register with a nickname, join channels, and exchange messages according to the IRC protocol (RFC 1459).  
It demonstrates the use of sockets, epoll (or poll/select equivalents), and proper client-server architecture while respecting C++98 standards.

---

## Instructions

### Compilation

```bash
# Clone the repository
git clone <repository_url>
cd ft_irc

# Compile the project using g++
make
Execution
# Start the server on a specific port with a password
./ircserv <PORT> <PASSWORD>

# Example:
./ircserv 6667 mysecretpass
```
### Connecting with HexChat

This server is designed to be used with HexChat.

1. Open HexChat

Launch HexChat and open the Network List.

2. Add a new network

Click Add and create a network named:

ft_irc
3. Configure the server

Click Edit and add a server entry:

localhost/6667

Replace 6667 if you started the server on a different port.

4. Set the server password

In the Server Password field enter the password used when launching the server:

mysecretpass
5. Connect

Click Connect.

HexChat will automatically send the required IRC registration commands:

PASS <password>
NICK <nickname>
USER <username> 0 * :<realname>

If registration is successful, the server will respond with the IRC welcome numeric replies.
## Resources

### References

RFC 1459 – Internet Relay Chat Protocol

Beej’s Guide to Network Programming

Linux man pages: socket(2), bind(2), listen(2), accept(2), recv(2), send(2), epoll(7)

## AI Usage

AI assistance (ChatGPT) was used to:

Explain concepts and best practices for epoll and non-blocking sockets

Suggest proper error handling patterns

Write this Readme.md with proper syntax and grammar.

AI was not used to generate the final project code, only as guidance for design, architecture, and clarifying technical questions.

## Features

Non-blocking server using epoll (or poll)

Handles multiple clients concurrently

IRC command parsing for PASS, NICK, USER

Registration with numeric replies (001–004)

Channel management (JOIN, PART, PRIVMSG)

Robust error handling and client disconnect management

## Technical Choices

C++98 compliant code, no modern C++ features

Low-level socket programming with TCP

Event-driven architecture using epoll for scalability

Per-client input buffer to handle partial messages

Clear separation of Server, Client, and Channel classes

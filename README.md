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
### Client Connection
You can connect using any IRC client (e.g., irssi, HexChat, WeeChat) or using nc (netcat) for testing:

nc localhost 6667

After connecting, the client must send the following commands in order:

PASS <password>

NICK <nickname>

USER <username> 0 * :<realname>

The server will then respond with numeric welcome messages (001–004) indicating successful registration.

## Resources

### References

RFC 1459 – Internet Relay Chat Protocol

Beej’s Guide to Network Programming

Linux man pages: socket(2), bind(2), listen(2), accept(2), recv(2), send(2), epoll(7)

## AI Usage

AI assistance (ChatGPT) was used to:

Explain concepts and best practices for epoll and non-blocking sockets

Suggest proper error handling patterns

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

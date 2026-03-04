#include "Client.hpp"
#include <sstream>

void handleMode(Client* client, const std::string& line) {
	std::stringstream linestream(line);
	std::string dummy;
	std::string target;

	dummy.reserve(4);
	linestream >> dummy >> target;
}

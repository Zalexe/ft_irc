#pragma once

#include <string>
#include <cstring>

std::string quitMessage(char* reason);
//int CheckArgs(char** argv);
bool wildcard_match(const char* str, const char* pattern);
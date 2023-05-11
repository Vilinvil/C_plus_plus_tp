#pragma once

#include <stdexcept>
#include <string>
#include <vector>

struct Command {
    std::string str();

    std::string operation_;
    std::string arg_;
};

// ParsePipe() can throw std::runtime_error
std::vector<Command> ParsePipe(std::string &pipe);
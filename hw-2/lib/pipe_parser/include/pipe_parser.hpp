#pragma once

#include <stdexcept>
#include <string>
#include <vector>

class Command {
  public:
    Command(const std::string &operation, const std::string &arg)
        : operation_(operation), arg_(arg){};

    std::string str();

    std::string operation_;
    std::string arg_;
};

// ParsePipe() can throw std::runtime_error
std::vector<Command> ParsePipe(std::string &pipe);
#pragma once

#include <stdexcept>
#include <string>
#include <vector>

class Command {
  public:
    Command(const std::string &operation, const std::string &arg)
        : operation_(operation), arg_(arg){};

    std::string str() { return ("Operation: " + operation_ + "arg: " + arg_); };

    std::string operation_;
    std::string arg_;
};

static bool isHaveQuotes(const std::string &str);

static void deleteQuotes(std::string &str);

// deleteToLastEcho doing nothing if not found echo command
static void deleteToLastEcho(std::string &str);

// ParseCommand can throw std::runtime_error and std::out_of_range
static Command ParseCommand(const std::string &command);

// ParsePipe() can throw std::runtime_error
std::vector<Command> ParsePipe(std::string &pipe);
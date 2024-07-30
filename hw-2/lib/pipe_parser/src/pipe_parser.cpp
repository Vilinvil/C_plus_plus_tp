#include "pipe_parser.hpp"

std::string Command::str() {
    return ("Operation: " + operation_ + "arg: " + arg_);
};

namespace {
constexpr std::string_view SEP = " | ";

// deleteToLastEcho doing nothing if not found echo command
void deleteToLastEcho(std::string &str) {
    if (size_t idxLastEcho = str.rfind("echo ");
        idxLastEcho != std::string::npos) {
        str.erase(0, idxLastEcho);
    }
}

// ParseCommand can throw std::runtime_error and std::out_of_range
Command ParseCommand(const std::string &command) {
    size_t idxGap = command.find(" ", 0);
    if (idxGap == std::string::npos) {
        throw std::runtime_error(
            "in ParseCommand: command have uncorrect format. In command: " +
            command + " For help use ./hw2 --help");
    }

    auto res = Command();
    res.operation_ = command.substr(0, idxGap);
    res.arg_ = command.substr(idxGap + 1);

    return res;
}
}   // namespace

// ParsePipe() can throw std::runtime_error
std::vector<Command> ParsePipe(std::string &pipe) {
    try {
        // It`s needed for optimization(we don't want handle commands because
        // last echo erase them)
        ::deleteToLastEcho(pipe);

        std::vector<Command> res;
        for (size_t idx_sep_command = pipe.find(::SEP);
             idx_sep_command != std::string::npos;
             idx_sep_command = pipe.find(::SEP)) {
            std::string command_str = pipe.substr(0, idx_sep_command);

            res.push_back(::ParseCommand(command_str));
            pipe.erase(0, idx_sep_command + 3);
        }

        res.push_back(::ParseCommand(pipe));
        return res;
    } catch (std::exception &e) {
        throw std::runtime_error("in ParsePipe: exception: " +
                                 static_cast<std::string>(e.what()));
    }
}

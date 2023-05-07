#include "pipe_parser.hpp"

std::string Command::str() {
    return ("Operation: " + operation_ + "arg: " + arg_);
};

namespace {
constexpr std::string_view SEP = " | ";

void deleteQuotes(std::string &str) {
    str.erase(0, 1);
    str.erase(str.length() - 1, 1);
}

// deleteToLastEcho doing nothing if not found echo command
void deleteToLastEcho(std::string &str) {
    if (size_t idxLastEcho = str.rfind("echo ");
        idxLastEcho != std::string::npos) {
        str.erase(0, idxLastEcho);
    }
}

bool isHaveQuotes(const std::string &str) {
    return str[0] == str[str.length() - 1] && str[0] == '\'';
}

// ParseCommand can throw std::runtime_error and std::out_of_range
Command ParseCommand(const std::string &command) {
    size_t idxGap = command.find(" ", 0);
    if (idxGap == std::string::npos) {
        throw std::runtime_error(
            "in ParseCommand: command have uncorrect format. In command: " +
            command + " For help use ./hw2 --help");
    }

    return Command(command.substr(0, idxGap), command.substr(idxGap + 1));
}
}   // namespace

// ParsePipe() can throw std::runtime_error
std::vector<Command> ParsePipe(std::string &pipe) {
    try {
        if (!::isHaveQuotes(pipe)) {
            throw std::runtime_error(
                "in PiplineHandler: pipe haven`t '' in start and end. For "
                "help use ./hw2 --help");
        }

        ::deleteQuotes(pipe);

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

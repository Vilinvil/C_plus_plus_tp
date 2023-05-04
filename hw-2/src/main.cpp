#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "operations.hpp"
#include "pipe_parser.hpp"

// PiplineHandler() can throw std::runtime_error
void PiplineHandler(std::string &pipe, std::ostream &out) {
    try {
        IOperationUP head;
        IOperation *tail;

        std::vector<Command> command_v = ParsePipe(pipe);

        bool flagFirst = true;
        for (auto it = command_v.begin(); it != command_v.end(); it++) {
            IOperation *tmp;
            if ((*it).operation_ == "echo") {
                tmp = new EchoOperation((*it).arg_, out);
            } else if ((*it).operation_ == "cat") {
                tmp = new CatOperation((*it).arg_, out);
            } else if ((*it).operation_ == "wc") {
                tmp = new WCOperation((*it).arg_, out);
            } else {
                throw std::runtime_error("Not supported command: " +
                                         (*it).str());
            }

            if (flagFirst) {
                head = std::unique_ptr<IOperation>(tmp);
                tail = head.get();
                flagFirst = false;
                continue;
            }

            tail->SetNextOperation(tmp);
            tail = tmp;
        }
        head->HandleEndOfInput();
    } catch (std::exception &e) {
        throw std::runtime_error("in PiplineHandler: exeption: " +
                                 static_cast<std::string>(e.what()));
    }
}

int main(int argc, char *argv[]) {
    std::cout << argc << " argv " << argv[1] << std::endl;
    std::string pipe = argv[1];
    try {
        PiplineHandler(pipe, std::cout);
    } catch (std::exception &e) {
        throw std::runtime_error("in main: exeption: " +
                                 static_cast<std::string>(e.what()));
    }
    return 0;
}
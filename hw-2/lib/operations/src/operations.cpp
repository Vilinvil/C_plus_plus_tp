#include "operations.hpp"

void EchoOperation::ProcessLine(const std::string &str){
    // Because Echo just ignore input lines
};

void EchoOperation::HandleEndOfInput() {
    IOperation *next = this->GetRefNext();
    if (next) {
        (*next).ProcessLine(arg_);
        (*next).HandleEndOfInput();
        return;
    }

    out_ << arg_;
};

void CatOperation::ProcessLine(const std::string &str) {
    IOperation *next = this->GetRefNext();
    if (next) {
        next->ProcessLine(str);
        return;
    }

    out_ << str;
};

// CatOperation::HandleEndOfInput() can throw std::runtime_error
void CatOperation::HandleEndOfInput() {
    try {
        IOperation *next = this->GetRefNext();
        std::ifstream ifs(arg_);
        std::string line;
        if (!ifs.is_open()) {
            throw std::runtime_error("in CatOperation::HandleEndOfInput(): ifs "
                                     "can`t open file with path: " +
                                     arg_);
        }

        while (ifs.is_open() && getline(ifs, line)) {
            if (next) {
                (*next).ProcessLine(line + "\n");
                continue;
            }

            out_ << line << std::endl;
        }

        if (next) {
            (*next).HandleEndOfInput();
        }

    } catch (std::exception &e) {
        throw std::runtime_error(
            "in CatOperation::HandleEndOfInput: exception: " +
            static_cast<std::string>(e.what()));
    }
};

void WCOperation::ProcessLine(const std::string &str) {
    counter_.AddCounter(str);
}

void WCOperation::HandleEndOfInput() {
    IOperation *next = this->GetRefNext();
    if (next) {
        (*next).ProcessLine(std::to_string(counter_.totalBite_));
        (*next).HandleEndOfInput();
        return;
    }

    out_ << counter_.totalBite_;
}

#include "operations.hpp"

void OperationWithNext::SetNextOperation(IOperation *operation) {
    next_ = std::unique_ptr<IOperation>(operation);
}

void EchoOperation::ProcessLine(const std::string &str){
    // Because Echo just ignore input lines
};

void EchoOperation::HandleEndOfInput() {
    IOperation *next = next_.get();
    if (next) {
        next->ProcessLine(arg_);
        next->HandleEndOfInput();
        return;
    }

    out_ << arg_;
};

void CatOperation::ProcessLine(const std::string &str) {
    IOperation *next = next_.get();
    if (next) {
        next->ProcessLine(str);
        return;
    }

    out_ << str;
};

// CatOperation::HandleEndOfInput() can throw std::runtime_error
void CatOperation::HandleEndOfInput() {
    try {
        IOperation *next = next_.get();
        std::ifstream ifs(arg_);
        std::string line;
        if (!ifs.is_open()) {
            throw std::runtime_error("in CatOperation::HandleEndOfInput(): ifs "
                                     "can`t open file with path: " +
                                     arg_);
        }

        while (getline(ifs, line)) {
            if (next) {
                next->ProcessLine(line + "\n");
                continue;
            }

            out_ << line << std::endl;
        }

        if (next) {
            next->HandleEndOfInput();
        }

    } catch (std::exception &e) {
        throw std::runtime_error(
            "in CatOperation::HandleEndOfInput: exception: " +
            static_cast<std::string>(e.what()));
    }
};

WCOperation::WCOperation(const std::string &arg, std::ostream &out)
    : BaseOperation(arg, out) {
    if (arg != "-c") {
        throw std::runtime_error(
            "in WCOperation constructor: not supported argument with "
            "command WC. Argument: " +
            arg + " For help use ./hw2 --help");
    }
};

void WCOperation::ProcessLine(const std::string &str) {
    counter_.AddCounter(str);
}

void WCOperation::HandleEndOfInput() {
    IOperation *next = next_.get();
    if (next) {
        next->ProcessLine(std::to_string(counter_.totalBite_));
        next->HandleEndOfInput();
        return;
    }

    out_ << counter_.totalBite_;
}

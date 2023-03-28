#pragma once

#include <stdexcept>
#include <string>

class lineError : std::runtime_error {
    using std::runtime_error::runtime_error;

  public:
    std::string getStr() { return this->what(); }
};
#pragma once

#include <stdexcept>
#include <string>

class LineError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};
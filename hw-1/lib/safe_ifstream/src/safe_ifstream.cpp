#include "safe_ifstream.hpp"

safeIfstream::safeIfstream(const std::string &filename) : ifs_(filename) {}

safeIfstream::~safeIfstream() {
    if (ifs_.is_open()) {
        ifs_.close();
    }
}

std::ifstream &safeIfstream::get() { return ifs_; }
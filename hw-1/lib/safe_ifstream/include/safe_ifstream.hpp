#pragma once

#include <fstream>
#include <string>

class safeIfstream {
  public:
    safeIfstream(const std::string &filename);
    ~safeIfstream();

    std::ifstream &get();

  private:
    std::ifstream ifs_;
};
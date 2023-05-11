#pragma once

#include <string_view>

inline constexpr std::string_view MES_HELP =
    R"(Command should be used in format: 
./hw2 '[command1] [argument1] | [command2] [argument2] | ...'
 echo write any text
 cat write file contens
 wc -c count bytes'")";

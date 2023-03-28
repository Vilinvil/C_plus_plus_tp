#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

// #include "utils.hpp"

#define MESSAGE_BAD_ARGUMENTS                                                  \
    "Bad request. Write --help or -h to see how utility work"

#define MESSAGE_HELP                                                           \
    "Command should be used in format: \
hw1 --path_artist|-a [your path to artist file] --path_type|-t [your path to artist_type] \
--query|-q [your query for searh] right only in this order"

class lineError : std::runtime_error {
    using std::runtime_error::runtime_error;

  public:
    std::string getStr() { return this->what(); }
};

// request arguments
struct reqArguments {
    std::string path_artist;
    std::string path_type;
    std::string query;
};

// getElem returns one element (all before first tab)
std::string getElem(std::string line) {
    size_t tabIdx = line.find("\t", 0);
    if (tabIdx == std::string::npos) {
        throw lineError("in getElem can`t find \\t");
    }

    std::string res;
    try {
        res = line.substr(0, tabIdx);
    } catch (std::out_of_range &e) {
        throw lineError("in getElem can`t erase because std::out_of_range");
    }

    return res;
}

// eraseToTabs erase count elements with tab in line.
void eraseToTabs(std::string &line, int count) {
    for (int i = 0; i < count; i++) {
        size_t tabIdx = line.find("\t", 0);
        if (tabIdx == std::string::npos) {
            throw lineError("in eraseToTabs can`t find \\t");
        }

        try {
            line.erase(0, tabIdx + 1);
        } catch (std::out_of_range &e) {
            throw lineError(
                "in eraseToTabs can`t erase because std::out_of_range");
        }
    }
}

int checkIdGroup(std::string line) {
    std::string id_str;
    try {
        id_str = getElem(line);
    } catch (lineError &e) {
        throw lineError("in checkIdGroup " + e.getStr());
    }

    try {
        eraseToTabs(line, 1);
    } catch (lineError &e) {
        throw lineError("in checkIdGroup " + e.getStr());
    }

    std::string type;
    try {
        type = getElem(line);
    } catch (lineError &e) {
        throw lineError("in checkIdGroup " + e.getStr());
    }

    if (type == "Group") {
        int res;
        try {
            res = std::stoi(id_str);
        } catch (std::exception &e) {
            throw lineError("in checkIdGroup can`t stoi");
        }
        return res;
    }

    return -1;
}

std::string getDataGroup(std::string line) {
    std::string year = getElem(line);
    eraseToTabs(line, 1);

    std::string month = getElem(line);
    eraseToTabs(line, 1);

    std::string day = getElem(line);

    std::string res;
    if (year == "\\N") {
        res = "_:";
    } else {
        res = year + ":";
    }

    if (month == "\\N") {
        res += "_:";
    } else {
        res = month + ":";
    }

    if (day == "\\N") {
        res += "_";
    } else {
        res = day;
    }

    return res;
}

std::string checkDateGroup(std::string line, std::string query, int id_group) {
    eraseToTabs(line, 2);
    std::string name = getElem(line);
    if (name != query) {
        // мб заменить на возвращение пустой и выкидывание эксепшена (does not
        // satisfy the query)
        return "";
    }

    eraseToTabs(line, 2);
    std::string data = getDataGroup(line);

    eraseToTabs(line, 6);
    // TODO check exceptions
    int type = std::stoi(getElem(line));

    if (type == id_group) {
        return data;
    }

    // мб заменить на возвращение пустой и выкидывание эксепшена "is not group"
    return "";
}

int run(reqArguments &req_arg, std::ostream &output) {
    std::ifstream in_type(req_arg.path_type);
    std::string line;
    int id_group;
    while (in_type.is_open() && getline(in_type, line)) {
        id_group = checkIdGroup(line);
        if (id_group != -1) {
            break;
        }
    }
    in_type.close();

    if (id_group == -1) {
        // мб throw
        return -1;
    }

    std::ifstream in_artist(req_arg.path_artist);
    std::string data_group;
    while (in_artist.is_open() && getline(in_artist, line)) {
        data_group = checkDateGroup(line, req_arg.query, id_group);
        if (data_group != "") {
            break;
        }
    }
    output << data_group;
    in_artist.close();
    return 0;
};

int main(int argc, char *argv[]) {
    std::cout << " argc = " << argc << std::endl;

    if (argc == 2) {
        std::string flag1 = argv[1];
        std::cout << argv[1] << std::endl;

        if (flag1 == "--help" || flag1 == "-h") {
            std::cout << MESSAGE_HELP;
            return 0;
        }
    }

    if (argc == 7) {
        std::string flag1 = argv[1];
        std::string flag2 = argv[3];
        std::string flag3 = argv[5];
        std::cout << flag1 << std::endl;
        std::cout << flag2 << std::endl;
        std::cout << flag3 << std::endl;

        if ((flag1 == "--path_artist" || flag1 == "-a") &&
            (flag2 == "--path_type" || flag2 == "-t") &&
            (flag3 == "--query" || flag3 == "-q")) {
            reqArguments req_arg;
            req_arg.path_artist = argv[2];
            req_arg.path_type = argv[4];
            req_arg.query = argv[6];

            return run(req_arg, std::cout);
        }
    }

    std::cerr << MESSAGE_BAD_ARGUMENTS;
    return 1;
}
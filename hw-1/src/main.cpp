#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "errors.hpp"
#include "messages.hpp"

// getElem returns one element (all before first tab)
// Can throw an exception lineError, std::runtime_error
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
// Can throw an exception lineError
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

// checkIdGroup checks id of group in line
// Can throw an exception lineError, std::runtime_error
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
            throw std::runtime_error("in checkIdGroup can`t stoi");
        }
        return res;
    }

    return -1;
}

// getDateGrop gets date of group from line
// Can throw an exception lineError
std::string getDateGroup(std::string line) {
    std::string year;
    try {
        year = getElem(line);
    } catch (lineError &e) {
        throw lineError("in getDateGroup " + e.getStr());
    }

    try {
        eraseToTabs(line, 1);
    } catch (lineError &e) {
        throw lineError("in getDateGroup " + e.getStr());
    }

    std::string month;
    try {
        month = getElem(line);
    } catch (lineError &e) {
    }

    try {
        eraseToTabs(line, 1);
    } catch (lineError &e) {
        throw lineError("in getDateGroup " + e.getStr());
    }

    std::string day = getElem(line);
    try {
        day = getElem(line);
    } catch (lineError &e) {
        throw lineError("in getDateGroup " + e.getStr());
    }

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

// checkDateGroup checks exist of group
// Can throw an exception lineError, std::runtime_error
std::string checkDateGroup(std::string line, std::string query, int id_group) {
    try {
        eraseToTabs(line, 2);
    } catch (lineError &e) {
        throw lineError("in checkDateGroup " + e.getStr());
    }

    std::string name;
    try {
        name = getElem(line);
    } catch (lineError &e) {
        throw lineError("in checkDateGroup " + e.getStr());
    }

    if (name != query) {
        return "";
    }

    try {
        eraseToTabs(line, 2);
    } catch (lineError &e) {
        throw lineError("in checkDateGroup " + e.getStr());
    }

    std::string data;
    try {
        data = getDateGroup(line);
    } catch (lineError &e) {
        throw lineError("in checkDateGroup " + e.getStr());
    }

    try {
        eraseToTabs(line, 6);
    } catch (lineError &e) {
        throw lineError("in checkDateGroup " + e.getStr());
    }

    std::string type_str;
    try {
        type_str = getElem(line);
    } catch (lineError &e) {
        throw lineError("in checkDateGroup " + e.getStr());
    }

    int type;
    try {
        type = std::stoi(type_str);
    } catch (std::exception &e) {
        throw std::runtime_error("in checkDateGroup can`t stoi");
    }

    if (type == id_group) {
        return data;
    }

    return "";
}

// request arguments
struct reqArguments {
    std::string path_artist;
    std::string path_type;
    std::string query;
};

// Can throw an exception lineError, std::runtime_error
int run(reqArguments &req_arg, std::ostream &output) {
    std::ifstream in_type(req_arg.path_type);
    std::string line;
    int id_group;
    while (in_type.is_open() && getline(in_type, line)) {
        try {
            id_group = checkIdGroup(line);
        } catch (lineError &e) {
            in_type.close();
            throw lineError("in run " + e.getStr());
        } catch (std::exception &e) {
            in_type.close();
            throw;
        }

        if (id_group != -1) {
            break;
        }
    }
    in_type.close();

    if (id_group == -1) {
        return -1;
    }

    std::ifstream in_artist(req_arg.path_artist);
    std::string data_group;
    while (in_artist.is_open() && getline(in_artist, line)) {
        try {
            data_group = checkDateGroup(line, req_arg.query, id_group);
        } catch (lineError &e) {
            in_artist.close();
            throw lineError("in checkDateGroup " + e.getStr());
        } catch (std::exception &e) {
            in_artist.close();
            throw;
        }

        if (data_group != "") {
            break;
        }
    }

    in_artist.close();
    if (data_group != "") {
        output << data_group;
    } else {
        output << "Not found";
    }

    return 0;
};

int main(int argc, char *argv[]) {
    if (argc == 2) {
        std::string flag1 = argv[1];

        if (flag1 == "--help" || flag1 == "-h") {
            std::cout << mes_help;
            return 0;
        }
    }

    if (argc == 7) {
        std::string flag1 = argv[1];
        std::string flag2 = argv[3];
        std::string flag3 = argv[5];

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

    std::cerr << mes_bad_argument;
    return 1;
}

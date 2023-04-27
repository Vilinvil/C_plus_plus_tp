#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "errors.hpp"
#include "messages.hpp"
#include "safe_ifstream.hpp"

// getElem returns one element (all before first tab)
// Can throw an exception LineError
std::string getElem(const std::string &line) {
    size_t tabIdx = line.find("\t", 0);
    if (tabIdx == std::string::npos) {
        throw LineError("in getElem: can`t find \\t");
    }

    std::string res;
    try {
        res = line.substr(0, tabIdx);
    } catch (std::out_of_range &e) {
        throw LineError("in getElem: can`t line.substr() because " +
                        static_cast<std::string>(e.what()));
    }

    return res;
}

// eraseToTabs erase count elements with tab in line.
// Can throw an exception LineError
void eraseToTabs(std::string &line, int count) {
    for (int i = 0; i < count; i++) {
        size_t tabIdx = line.find("\t", 0);
        if (tabIdx == std::string::npos) {
            throw LineError("in eraseToTabs: can`t find \\t");
        }

        try {
            line.erase(0, tabIdx + 1);
        } catch (std::out_of_range &e) {
            throw LineError("in eraseToTabs: can`t erase because " +
                            static_cast<std::string>(e.what()));
        }
    }
}

// getIdGroup checks id of group in line
// Can throw an exception LineError
int getIdGroup(std::string &line) {
    try {
        std::string id_str = getElem(line);
        eraseToTabs(line, 1);

        std::string type = getElem(line);

        if (type == "Group") {
            int res = std::stoi(id_str);
            return res;
        }

        return -1;
    } catch (std::exception &e) {
        throw LineError("in getIdGroup: " + static_cast<std::string>(e.what()));
    }
}

// getDateGrop gets date of group from line
// Can throw an exception LineError
std::string getDateGroup(std::string &line) {
    try {
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
    } catch (LineError &e) {
        throw LineError("in getDateGroup: " +
                        static_cast<std::string>(e.what()));
    }
}

// getDateGroup checks exist of group
// Can throw an exception LineError
std::string getDateGroup(std::string line, const std::string &query,
                         int id_group) {
    try {
        eraseToTabs(line, 2);
        std::string name = getElem(line);
        if (name != query) {
            return "";
        }

        eraseToTabs(line, 2);

        std::string data = getDateGroup(line);
        eraseToTabs(line, 4);

        std::string type_str = getElem(line);
        int type = std::stoi(type_str);
        if (type == id_group) {
            return data;
        }

        return "";
    } catch (std::exception &e) {
        throw LineError("in getDateGroup: " +
                        static_cast<std::string>(e.what()));
    }
}

// request arguments
struct reqArguments {
    std::string path_artist;
    std::string path_type;
    std::string query;
};

// Can throw an exception LineError
int run(const reqArguments &req_arg, std::ostream &output) {
    try {
        safeIfstream in_type_safe(req_arg.path_type);
        std::ifstream &in_type = in_type_safe.get();

        std::string line;
        int id_group = -1;
        while (in_type.is_open() && getline(in_type, line) && id_group == -1) {
            id_group = getIdGroup(line);
        }
        in_type.close();

        if (id_group == -1) {
            return -1;
        }

        safeIfstream in_artist_safe(req_arg.path_artist);
        std::ifstream &in_artist = in_artist_safe.get();

        std::string data_group;
        while (in_artist.is_open() && getline(in_artist, line) &&
               data_group == "") {
            data_group = getDateGroup(line, req_arg.query, id_group);
        }

        in_artist.close();
        if (data_group != "") {
            output << data_group;
        } else {
            output << "Not found";
        }

        return 0;
    } catch (LineError &e) {
        throw LineError("in run LineError: " +
                        static_cast<std::string>(e.what()));
    } catch (std::exception &e) {
        throw std::runtime_error("in run: " +
                                 static_cast<std::string>(e.what()));
    }
}

int main(int argc, char *argv[]) {
    std::cout << argv[0];
    std::cout << argv[1] << std::endl
              << argv[2] << std::endl
              << argv[3] << std::endl
              << argv[4] << std::endl
              << argv[5] << std::endl
              << argv[6] << std::endl;
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

            int res;
            try {
                res = run(req_arg, std::cout);
            } catch (std::exception &e) {
                std::cerr << "in main: " << e.what();
            }
            return res;
        }
    }

    std::cerr << mes_bad_argument;
    return 1;
}

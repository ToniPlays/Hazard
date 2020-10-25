#include <hzrpch.h>
#include "StringUtils.h"

namespace Hazard {

    std::vector<std::string> StringUtils::SplitString(std::string string, char delim) {

        std::vector<std::string> result;

        std::istringstream f(string);
        std::string s;

        while (getline(f, s, delim)) {
            result.push_back(s);
        }

        return result;
    }
}
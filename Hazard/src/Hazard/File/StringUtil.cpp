
#include <hzrpch.h>
#include "StringUtil.h"

namespace Hazard::Utility {

    std::vector<std::string> StringUtil::SplitString(std::string string, char delim) {

        std::vector<std::string> result;
        std::istringstream f(string);
        std::string s;

        while (getline(f, s, delim)) {
            result.push_back(s);
        }
        return result;
    }
}

#include <hzrpch.h>
#include "StringUtil.h"

namespace Hazard {

    std::vector<std::string> StringUtil::SplitString(std::string string, char delim) {

        std::vector<std::string> result;
        std::istringstream f(string);
        std::string s;

        while (getline(f, s, delim)) {
            result.push_back(s);
        }
        return result;
    }
    void StringUtil::RemoveAtEnd(std::string& source, char val)
    {
        source = source.substr(0, source.find_first_of(val));
    }
}
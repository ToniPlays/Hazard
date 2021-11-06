
#include <hzrpch.h>
#include "StringUtil.h"

namespace Hazard {

	std::vector<std::string> StringUtil::SplitString(std::string string, char delim) {

		std::vector<std::string> result;
		std::istringstream f(string);
		std::string s;

		while (getline(f, s, delim)) {
			if (!s.empty()) 
				result.push_back(s);
		}
		return result;
	}
	std::string StringUtil::RemoveAtEnd(std::string& source, char val)
	{
		return source.substr(0, source.find_first_of(val));
	}
	std::string StringUtil::BytesToString(const size_t& bytes)
	{
		constexpr uint64_t GB = 1024 * 1024 * 1024;
		constexpr uint64_t MB = 1024 * 1024;
		constexpr uint64_t KB = 1024;

		char buffer[16];
		if (bytes > GB) sprintf_s(buffer, "%.02f GB", (float)bytes / (float)GB);
		else if (bytes > MB) sprintf_s(buffer, "%.02f MB", (float)bytes / (float)MB);
		else if (bytes > KB) sprintf_s(buffer, "%.02f KB", (float)bytes / (float)KB);
		else sprintf_s(buffer, "%.02f bytes", (float)bytes);
		return std::string(buffer);
	}
}
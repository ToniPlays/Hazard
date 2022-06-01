
#include "StringUtil.h"
#include <sstream>


std::vector<std::string> StringUtil::SplitString(const std::string& string, char delim) {

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

	char buffer[32];
	if (bytes > GB) sprintf(buffer, "%.02f GB", (float)bytes / (float)GB);
	else if (bytes > MB) sprintf(buffer, "%.02f MB", (float)bytes / (float)MB);
	else if (bytes > KB) sprintf(buffer, "%.02f KB", (float)bytes / (float)KB);
	else sprintf(buffer, "%.02f bytes", (float)bytes);
	return std::string(buffer);
}
std::string StringUtil::Replace(const std::string& value, const std::string& find, const std::string& replaceWith)
{
	if (find.empty())
		return value;

	std::string result = value;
	size_t pos = 0;
	while ((pos = value.find(find, pos)) != std::string::npos)
	{
		result.replace(pos, strlen(find.c_str()), replaceWith);
		pos += strlen(find.c_str());
	}

	return result;
}
std::string& StringUtil::ToLower(std::string& string)
{
	std::transform(string.begin(), string.end(), string.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return string;
}
bool StringUtil::IsMatching(const std::string& value, const std::string& compareTo, bool caseSensitive, bool stripWhiteSpaces, bool stripUnderScores) {
	if (compareTo.empty())
		return true;

	if (value.empty())
		return false;

	std::string nameSanitized = stripUnderScores ? Replace(value, "_", " ") : value;

	if (stripWhiteSpaces)
		nameSanitized = Replace(nameSanitized, " ", "");

	std::string searchString = stripWhiteSpaces ? Replace(compareTo, " ", "") : compareTo;

	if (!caseSensitive)
	{
		nameSanitized = ToLower(nameSanitized);
		searchString = ToLower(searchString);
	}

	return Contains(nameSanitized, searchString);
}

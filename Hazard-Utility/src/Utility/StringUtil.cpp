
#include "StringUtil.h"
#include <sstream>


std::vector<std::string> StringUtil::SplitString(const std::string& string, char delim) 
{

	std::vector<std::string> result;
	std::istringstream f(string);
	std::string s;

	while (getline(f, s, delim)) 
	{
		if (!s.empty())
			result.push_back(s);
	}
	return result;
}
std::string StringUtil::RemoveAtEnd(std::string& source, char val)
{
	return source.substr(0, source.find_first_of(val));
}
size_t StringUtil::OffsetOf(const std::string& source, char delim, size_t offset)
{
	std::string offsetValue = source;

	for (uint32_t i = 0; i < offset; i++) {
		offsetValue = offsetValue.substr(offsetValue.find_first_of(delim) + 1);
	}
	return offsetValue.find_first_of(delim, offset);
}

size_t StringUtil::OffsetOf(const std::string& source, const std::string& value, size_t startOffset) 
{
	return source.find(value, startOffset);
}

std::string StringUtil::BytesToString(const size_t& bytes)
{
	constexpr uint64_t GB = 1024 * 1024 * 1024;
	constexpr uint64_t MB = 1024 * 1024;
	constexpr uint64_t KB = 1024;
	constexpr uint64_t bufSize = 32;

	char buffer[bufSize];
	if (bytes > MB) snprintf(buffer, bufSize, "%.0f GB", (float)bytes / (float)GB);
	else if (bytes > MB) snprintf(buffer, bufSize, "%.2f MB", (float)bytes / (float)MB);
	else if (bytes > KB) snprintf(buffer, bufSize, "%.2f KB", (float)bytes / (float)KB);
	else snprintf(buffer, bufSize, "%.2f bytes", (float)bytes);
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
bool StringUtil::IsMatching(const std::string& value, const std::string& compareTo, bool caseSensitive, bool stripWhiteSpaces, bool stripUnderScores) 
{
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
std::string_view StringUtil::Between(const std::string_view& source, const std::string& start, const std::string& end)
{
	size_t startPos = source.find(start);
	size_t endPos = source.find(end, startPos + 1);

	return source.substr(startPos + start.length(), endPos - startPos - end.length());
}
std::string StringUtil::GetPreprocessor(const char* type, const std::string& source, size_t endPosition, size_t* offset)
{
	size_t valueOffset = OffsetOf(source, type, endPosition);

	if (valueOffset == std::string::npos) 
	{
		*offset = std::string::npos;
		return "";
	}

	size_t lineEndOffset = source.find_first_of("\r\n", valueOffset);
	std::string line = source.substr(valueOffset, lineEndOffset - valueOffset);
	*offset = valueOffset + line.length();
	return line.substr(line.find_first_of(' ') + 1);
}

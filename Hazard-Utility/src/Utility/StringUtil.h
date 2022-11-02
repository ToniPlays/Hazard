#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include <istream>
#include <string>

class StringUtil 
{
public:
	static std::vector<std::string> SplitString(const std::string& string, char delim);
	static std::string RemoveAtEnd(std::string& source, char val);
	static size_t OffsetOf(const std::string& source, char delim, size_t startOffset = 0);
	static size_t OffsetOf(const std::string& source, const std::string& value, size_t startOffset = 0);
	static std::string BytesToString(const size_t& bytes);
	static std::string Replace(const std::string& value, const std::string& find, const std::string& replaceWith);
	static std::string& ToLower(std::string& string);
	static bool IsMatching(const std::string& value, const std::string& compareTo, bool caseSensitive = false, bool stripWhiteSpaces = true, bool stripUnderScores = true);
	static bool Contains(std::string_view source, std::string_view find) { return source.find(find) != std::string::npos; }
	static std::string_view Between(const std::string_view& source, const std::string& start, const std::string& end);

	static std::string GetPreprocessor(const char* type, const std::string& source, size_t endPosition, size_t* offset = nullptr);

	template<typename T>
	static std::string FormatNumber(T number) {
		std::string num = std::to_string(number);
		int n = (int)num.length() - 3;

		while (n > 0) {
			num.insert(n, ",");
			n -= 3;
		}
		return num;
	}
};

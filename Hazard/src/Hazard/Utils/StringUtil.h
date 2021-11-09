#pragma once
#include "Hazard/Core/Core.h"

namespace Hazard 
{
	class StringUtil {

	public:
		static std::vector<std::string> SplitString(std::string string, char delim);
		static std::string RemoveAtEnd(std::string& source, char val);
		static std::string BytesToString(const size_t& bytes);
		static std::string Replace(const std::string& value, const std::string& find, const std::string& replace);
		/*
		Split source by token, returns type and source until next type token
		*/
		template<typename T>
		static void PreprocessTypeSource(const std::string& source, const char* token, T fn)
		{
			size_t tokenLen = strlen(token);
			size_t pos = source.find(token, 0);

			while (pos != std::string::npos)
			{
				size_t eol = source.find_first_of("\r\n", pos);
				size_t begin = pos + tokenLen + 1;
				std::string type = source.substr(begin, eol - begin);
				size_t nextLinePos = source.find_first_not_of("\r\n", eol);
				pos = source.find(token, nextLinePos);
				std::string tokeSource = pos == std::string::npos ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
				fn(type, tokeSource);
			}
		}

		template<typename T>
		static std::string FormatNumber(T number) {
			std::string num = std::to_string(number);
			size_t n = num.length() - 3;

			while (n > 0) {
				num.insert(n, ",");
				n -= 3;
			}
			return num;
		}
	};
}
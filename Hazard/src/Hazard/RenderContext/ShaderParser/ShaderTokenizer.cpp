#include "ShaderTokenizer.h"


#include "spdlog/fmt/fmt.h"

namespace Hazard::Shading
{
	void ShaderTokenizer::Tokenize()
	{
		m_Tokens.clear();
		std::string source = File::ReadFile(m_SourcePath);

		uint32_t position = 0;

		try
		{
			while (position < source.length())
			{
				std::string remaining = source.substr(position);
				char currentChar = remaining[0];


				uint32_t parsed = 0;
				if (std::isspace(currentChar))
					parsed += ParseWhitespace(remaining);

				else if (currentChar == m_Defs.ScopeBegin)
				{
					m_Tokens.push_back({ TokenType::ScopeBegin, remaining.substr(0, 1), m_Line });
					parsed++;
				}

				else if (currentChar == m_Defs.ScopeEnd)
				{
					m_Tokens.push_back({ TokenType::ScopeEnd, remaining.substr(0, 1), m_Line });
					parsed++;
				}

				else if (remaining.starts_with(m_Defs.SingleLineComment))
					parsed += ParseComment(remaining);

				else if (remaining.starts_with(m_Defs.MultiLineCommentStart))
					parsed += ParseComment(remaining);

				else if (std::isdigit(currentChar))
					parsed += ParseNumber(remaining);

				else if (remaining.starts_with('\"'))
					parsed += ParseString(remaining);

				if (parsed == 0)
					parsed = ParseOperator(remaining);
				if (parsed == 0)
					parsed = ParseSymbol(remaining);
                if (parsed == 0)
                    parsed = ParseKeyword(remaining);
				if (parsed == 0)
					parsed = ParseIdentifier(remaining);
				

				if (parsed > 0)
				{
					position += parsed;
					continue;
				}

				std::string line = remaining.substr(0, remaining.find_first_of(" \n"));
				throw CompileException(m_Line, fmt::format("Unrecognized \"{}\" at {}", line, m_Line));
			}
		}
		catch (CompileException& e)
		{
			m_Exception = e;
		}
	}

	uint64_t ShaderTokenizer::ParseWhitespace(const std::string& source)
	{
		uint64_t i = 0;
		while (i < source.length() && std::isspace(source[i]))
		{
			if (source[i] == '\t')
				m_Line++;

			i++;
		}

		m_Tokens.push_back({ TokenType::Whitespace, source.substr(0, i) });
		return i;
	}

	uint64_t ShaderTokenizer::ParseComment(const std::string& source)
	{
		bool isMultiline = source.starts_with(m_Defs.MultiLineCommentStart);

		if (!isMultiline)
		{
			uint32_t startLen = m_Defs.SingleLineComment.length();
			uint64_t length = source.find("\n");
			m_Tokens.push_back({ TokenType::Comment, source.substr(startLen, length - 1), m_Line });
			return length + startLen;
		}

		uint32_t startLen = m_Defs.MultiLineCommentStart.length();
		uint64_t length = source.find(m_Defs.MultiLineCommentEnd);

		m_Tokens.push_back({ TokenType::Comment, source.substr(startLen, length - 2), m_Line });
		return length + startLen + m_Defs.MultiLineCommentEnd.length();
	}

	uint64_t ShaderTokenizer::ParseString(const std::string& source)
	{
		uint64_t index = 1;
		while (index < source.length() && source[index] != '"' || source[index - 1] == '\\')
			index++;

		m_Tokens.push_back({ TokenType::String, source.substr(1, index - 1), m_Line });
		return index + 1;
	}

	uint64_t ShaderTokenizer::ParseNumber(const std::string& source)
	{
		uint64_t index = 0;
		while (index < source.length() && !std::isspace(source[index]) && (std::isdigit(source[index]) || source[index] == '.'))
			index++;

		m_Tokens.push_back({ TokenType::Number, source.substr(0, index), m_Line });
		return index;
	}

	uint64_t ShaderTokenizer::ParseSymbol(const std::string& source)
	{
		if (std::find(m_Defs.Symbols.begin(), m_Defs.Symbols.end(), source[0]) == m_Defs.Symbols.end())
			return 0;

		m_Tokens.push_back({ TokenType::Symbol, source.substr(0, 1), m_Line });
		return 1;
	}

	uint64_t ShaderTokenizer::ParseIdentifier(const std::string& source)
	{
		uint64_t index = 0;
		while (index < source.length() && !std::isspace(source[index]))
			index++;

		m_Tokens.push_back({ TokenType::Identifier, source.substr(0, index), m_Line });
		return index;
	}

	uint64_t ShaderTokenizer::ParseOperator(const std::string& source)
	{
		if (std::find(m_Defs.Operators.begin(), m_Defs.Operators.end(), source[0]) == m_Defs.Operators.end())
			return 0;

		m_Tokens.push_back({ TokenType::Operator, source.substr(0, 1), m_Line });
		return 1;
	}

	uint64_t ShaderTokenizer::ParseKeyword(const std::string& source)
	{
		uint64_t index = 0;
		while (index < source.length() && !std::isspace(source[index]))
			index++;

		std::string keyword = source.substr(0, index);

		if (std::find(m_Defs.Keywords.begin(), m_Defs.Keywords.end(), keyword) == m_Defs.Keywords.end())
            return 0;

		m_Tokens.push_back({ TokenType::Keyword, keyword, m_Line });
		return keyword.length() + 1;
	}
}

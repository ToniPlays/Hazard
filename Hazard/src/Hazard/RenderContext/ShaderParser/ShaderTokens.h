#pragma once

#include "UtilityCore.h"
#include <regex>

namespace Hazard::Shading
{
	enum class TokenType
	{
		Keyword,
		Identifier,
		Symbol,
		Number,
		String,
		Whitespace,
		Comment,
		Property,
		Attribute,
		ScopeBegin,
		ScopeEnd,
		Operator,
		Unknown
	};

	struct ShadingLanguageDefinitions
	{
		std::vector<std::string> Keywords = { "Shader", "Type", "Language", "Version", "Include", "Properties", "Constants", "Pass", 
			"Vertex", "Fragment", "Pixel", "Compute", "Raygen", "Miss", "Hit", "AnyHit", "default", "case", "break", "continue", "if", "else"
		};

		std::vector<char> Symbols = { ',', '[', ']', '(', ')', ';', ':', '_', '.' };
		std::vector<char> Operators = { '<', '>', '=', '!', '&', '|', '*', '/' };

		std::string SingleLineComment = "//";
		std::string MultiLineCommentStart = "/*";
		std::string MultiLineCommentEnd = "*/";
		char PreprocessorToken = '#';

		char ScopeBegin = '{';
		char ScopeEnd = '}';
	};

	struct ShaderParserToken
	{
		TokenType Type;
		std::string Value;
		uint32_t Line;
		uint32_t Index;
	};

	static std::string TokenTypeToString(TokenType type)
	{
		switch (type)
		{
			case Hazard::Shading::TokenType::Keyword:		return "Keyword";
			case Hazard::Shading::TokenType::Identifier:	return "Identifier";
			case Hazard::Shading::TokenType::Symbol:		return "Symbol";
			case Hazard::Shading::TokenType::Number:		return "Number";
			case Hazard::Shading::TokenType::String:		return "String";
			case Hazard::Shading::TokenType::Whitespace:	return "Whitespace";
			case Hazard::Shading::TokenType::Comment:		return "Comment";
			case Hazard::Shading::TokenType::Property:		return "Property";
			case Hazard::Shading::TokenType::Attribute:		return "Attribute";
			case Hazard::Shading::TokenType::ScopeBegin:	return "ScopeBegin";
			case Hazard::Shading::TokenType::ScopeEnd:		return "ScopeEnd";
			case Hazard::Shading::TokenType::Operator:		return "Operator";
			case Hazard::Shading::TokenType::Unknown:		return "Unknown";
		}
		return "Unknown";
	}
}
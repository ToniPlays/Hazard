#pragma once

#include "UtilityCore.h"
#include "ShaderTokens.h"

namespace Hazard::Shading
{
	class ShaderParser
	{
	public:
		ShaderParser(const std::vector<ShaderParserToken>& tokens) : m_Tokens(tokens) {};
		~ShaderParser() = default;

		void Parse();

		uint32_t GetCurrentLine() const { return m_CurrentLine; }

	private:
		uint32_t m_CurrentLine = 1;
		std::vector<ShaderParserToken> m_Tokens;
	};
}
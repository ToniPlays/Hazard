#include "ShaderParser.h"
#include <Utility/Hooks.h>

namespace Hazard::Shading
{
	void ShaderParser::Parse()
	{
		Hooks<TokenType, void(const ShaderParserToken&)> hooks;

		hooks.AddHook(TokenType::Whitespace, [this](const ShaderParserToken& token) {
			uint64_t index = 0;
			while ((index = token.Value.find_first_of('\n', index)) != std::string::npos) {
				index++;
				m_CurrentLine++;
			}
		});

		for (uint32_t i = 0; i < m_Tokens.size(); i++)
		{
			auto& token = m_Tokens[i];
			hooks.Invoke(token.Type, token);
		}
	}
}
#pragma once

#include "UtilityCore.h"
#include "ShaderTokens.h"
#include "ParseException.h"


namespace Hazard::Shading
{
    struct ATLNode
    {
        std::string Name;
        std::string Value;
        std::map<std::string, std::shared_ptr<ATLNode>> Children; // named child scopes or properties
        std::vector<std::shared_ptr<ATLNode>> ListChildren;       // multiple items with same name
    };

	class ShaderParser
	{
	public:
		ShaderParser(const std::vector<ShaderParserToken>& tokens) : m_Tokens(tokens), m_Index(0) {};
		~ShaderParser() = default;

        std::shared_ptr<ATLNode> Parse()
        {
            auto root = std::make_shared<ATLNode>();
            root->Name = "Root";
            while (m_Index < m_Tokens.size())
            {
                ParseToken(root);
            }
            return root;
        }
        
    private:
        void ParseToken(std::shared_ptr<ATLNode> parent);
        void ParseScope(std::shared_ptr<ATLNode> parent);

        uint32_t SkipNonSemanticTokens(uint32_t index);

        
	private:
        std::vector<ShaderParserToken> m_Tokens;
        uint32_t m_Index;
	};
}

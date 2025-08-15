#include <hzrpch.h>

#include "ShaderParser.h"
#include "../CompileException.h"
#include <Utility/Hooks.h>

#include "spdlog/fmt/fmt.h"

namespace Hazard::Shading
{
	void ShaderParser::Parse()
	{
        m_KeywordValueTypes = { { "Shader", TokenType::String },
                                { "Language", TokenType::String, },
                                { "Type", TokenType::String },
                                { "Version", TokenType::Number },
                                { "Depth", TokenType::String },
                                { "DepthWrite", TokenType::Identifier },
                                { "Properties", TokenType::Property },
                                { "Vertex", TokenType::Scope },
                                { "Fragment", TokenType::Scope },
                                { "Compute", TokenType::Scope },
                                { "Source", TokenType::Property },
                                { "Include", TokenType::Property },
        };
        
        uint32_t currentToken = 0;
		while(currentToken < m_Tokens.size())
            currentToken += ParseToken(currentToken);
        
        float i = 0;
	}

    uint32_t ShaderParser::ParseToken(uint32_t index)
    {
        ShaderParserToken& token = m_Tokens[index];
        switch(token.Type)
        {
            case TokenType::Keyword:
                return ParseKeyword(index);
            case TokenType::ScopeBegin:
            {
                GetCurrentScope()->Scopes[m_NextScopeName] = {};
                m_ScopeStack.push_back(m_NextScopeName);
                return 1;
            }
            case TokenType::ScopeEnd:
                m_NextScopeName = "scope";
                m_ScopeStack.pop_back();
                return 1;
            default: 
                break;
        }
        return 1;
    }

    uint32_t ShaderParser::ParseKeyword(uint32_t index)
    {
        ShaderParserToken& key = m_Tokens[index];
        ShaderParserToken& value = m_Tokens[index + 1];
        
        if(m_KeywordValueTypes.find(key.Value) == m_KeywordValueTypes.end())
            throw CompileException(key.Line, fmt::format("Unknow {}", key.Value));
        
        auto keywordType = m_KeywordValueTypes[key.Value];
        if(value.Type != keywordType)
        {
            if(keywordType == TokenType::Scope)
            {
                m_NextScopeName = key.Value;
                return 1;
            }
            
            if(keywordType == TokenType::Property)
            {
                uint32_t i = 0;
                uint32_t scopeBegin = 0;
                while(i < m_Tokens.size())
                {
                    auto& token = m_Tokens[index + i];
                    i++;
                    if(token.Type == TokenType::ScopeBegin)
                    {
                        scopeBegin++;
                    }
                    else if (token.Type == TokenType::ScopeEnd)
                    {
                        scopeBegin--;
                        if(scopeBegin == 0)
                            break;
                    }
                    
                    if(scopeBegin > 0)
                        GetCurrentScope()->Values[key.Value] += token.Value;
                }
                return i + 1;
            }
            
            throw CompileException(key.Line, fmt::format("Value for {} must be {}, instead it was {}", key.Value, TokenTypeToString(keywordType), TokenTypeToString(value.Type)));
        }
        GetCurrentScope()->Values[key.Value] = value.Value;
        
        return 2;
    }
}

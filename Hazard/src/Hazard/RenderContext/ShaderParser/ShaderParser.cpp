#include <hzrpch.h>

#include "ShaderParser.h"
#include "../CompileException.h"
#include <Utility/Hooks.h>

#include "spdlog/fmt/fmt.h"

namespace Hazard::Shading
{
    void ShaderParser::ParseToken(std::shared_ptr<ATLNode> parent)
    {
        if (m_Index >= m_Tokens.size())
            return;

        const ShaderParserToken& token = m_Tokens[m_Index];

        switch (token.Type)
        {
            case TokenType::Identifier:
            case TokenType::Keyword:
            {
                std::string key = token.Value;
                m_Index = SkipNonSemanticTokens(m_Index + 1);

                // Check for a scope
                if (m_Index < m_Tokens.size() && m_Tokens[m_Index].Type == TokenType::ScopeBegin)
                {
                    m_Index++; // consume '{'
                    auto child = std::make_shared<ATLNode>();
                    child->Name = key;
                    if (key == "Include" || key == "Source")
                    {
                        std::ostringstream values;
                        int scopeDepth = 1;

                        while (m_Index < m_Tokens.size() && scopeDepth > 0)
                        {
                            const auto& t = m_Tokens[m_Index];

                            if (t.Type == TokenType::ScopeBegin)
                                scopeDepth++;
                            else if (t.Type == TokenType::ScopeEnd)
                                scopeDepth--;
                            else if (t.Type == TokenType::Comment)
                            {
                                m_Index++;
                                continue;
                            }
                            else if(t.Type == TokenType::Keyword)
                            {
                                values << t.Value << " ";
                                m_Index++;
                                continue;
                            }

                            if (scopeDepth > 0)
                                values << t.Value;

                            m_Index++;
                        }

                        child->Value = values.str(); // preserve entire block exactly
                    }
                    else
                    {
                        ParseScope(child); // normal nested scope
                    }
                    parent->Children[key] = child;
                }
                // Check for multi-token parenthesis value
                // Check for multi-token parenthesis value
                else if (m_Index < m_Tokens.size() && m_Tokens[m_Index].Value == "(")
                {
                    m_Index++; // consume '('
                    std::ostringstream valueStream;
                    int parenCount = 1;

                    while (m_Index < m_Tokens.size() && parenCount > 0)
                    {
                        const auto& t = m_Tokens[m_Index];

                        if (t.Value == "(")
                            parenCount++;
                        else if (t.Value == ")")
                            parenCount--;
                        else if (t.Type != TokenType::Whitespace && t.Type != TokenType::Comment)
                            valueStream << t.Value << " ";  // preserve spacing

                        m_Index++;
                    }

                    // Trim trailing space
                    std::string value = valueStream.str();
                    if (!value.empty() && value.back() == ' ')
                        value.pop_back();

                    auto child = std::make_shared<ATLNode>();
                    child->Name = key;
                    child->Value = value;
                    parent->Children[key] = child;
                }

                // It's a single-value property
                else if (m_Index < m_Tokens.size())
                {
                    auto child = std::make_shared<ATLNode>();
                    child->Name = key;
                    child->Value = m_Tokens[m_Index].Value;
                    parent->Children[key] = child;
                    m_Index++;
                }
                break;
            }

            case TokenType::ScopeEnd:
                // Should never happen here; handled in ParseScope
                m_Index++;
                break;

            default:
                // Ignore whitespace, comments, etc.
                m_Index++;
                break;
        }
    }
    void ShaderParser::ParseScope(std::shared_ptr<ATLNode> parent)
    {
        while (m_Index < m_Tokens.size())
        {
            const ShaderParserToken& token = m_Tokens[m_Index];

            if (token.Type == TokenType::ScopeEnd)
            {
                m_Index++; // consume '}'
                return;
            }

            ParseToken(parent);
        }
    }
    uint32_t ShaderParser::SkipNonSemanticTokens(uint32_t index)
    {
        while (index < m_Tokens.size())
        {
            if (m_Tokens[index].Type == TokenType::Whitespace ||m_Tokens[index].Type == TokenType::Comment)
                index++;
            else
                break;
        }
        return index;
    }
}

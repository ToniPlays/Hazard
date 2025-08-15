#pragma once

#include "UtilityCore.h"
#include "ShaderTokens.h"
#include "ParseException.h"

struct ParsedScope {
    std::unordered_map<std::string, std::string> Values;
    std::unordered_map<std::string, ParsedScope> Scopes;
    
    template<typename T>
    T GetValueOrDefault(const char* value, T defaultValue)
    {
        if(Values.find(value) == Values.end())
            return defaultValue;
        
        if constexpr (std::is_same<T, std::string>::value)
            return Values[value];
        if constexpr (std::is_same<T, uint32_t>::value)
            return stoi(Values[value]);
    }
    
    template<typename T>
    bool RequireValue(const char* value, T& target)
    {
        if (Values.find(value) == Values.end())
            throw ParseException(fmt::format("Require value not found: {}", value));

        if constexpr (std::is_same<T, uint32_t>::value)
            target = stoi(Values[value]);
        else target = Values[value];

        return true;
    }
};

namespace Hazard::Shading
{
	class ShaderParser
	{
	public:
		ShaderParser(const std::vector<ShaderParserToken>& tokens) : m_Tokens(tokens) {};
		~ShaderParser() = default;

		void Parse();
        
        const ParsedScope& GetRootScope() const { return m_RootScope; }

		uint32_t GetCurrentLine() const { return m_CurrentLine; }
        
    private:
        uint32_t ParseToken(uint32_t index);
        uint32_t ParseKeyword(uint32_t index);
        
        ParsedScope* GetCurrentScope() {
            ParsedScope* scope = &m_RootScope;
            for(auto& key : m_ScopeStack)
                scope = &scope->Scopes.at(key);
            return scope;
        }
        
	private:
		uint32_t m_CurrentLine = 1;
        std::string m_NextScopeName = "scope";
		std::vector<ShaderParserToken> m_Tokens;
        std::unordered_map<std::string, TokenType> m_KeywordValueTypes;
        ParsedScope m_RootScope;
        std::vector<std::string> m_ScopeStack;
	};
}

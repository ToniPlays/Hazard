#pragma once

#include "Filesystem/File.h"
#include "ShaderTokens.h"

#include <Hazard/RenderContext/CompileException.h>

namespace Hazard::Shading
{
	class ShaderTokenizer
	{
	public:
		ShaderTokenizer(const std::filesystem::path& source) : m_SourcePath(source) {}
		~ShaderTokenizer() = default;

		void Tokenize();
		const std::vector<ShaderParserToken> GetTokens() const { return m_Tokens; }
		const std::optional<CompileException>& GetException() const { return m_Exception; }

	private:
		uint64_t ParseWhitespace(const std::string& source);
		uint64_t ParseComment(const std::string& source);
		uint64_t ParseString(const std::string& source);
		uint64_t ParseNumber(const std::string& source);
		uint64_t ParseSymbol(const std::string& source);
		uint64_t ParseIdentifier(const std::string& source);
		uint64_t ParseOperator(const std::string& source);
		uint64_t ParseKeyword(const std::string& source);

	private:
		std::filesystem::path m_SourcePath;
		std::vector<ShaderParserToken> m_Tokens;
		std::optional<CompileException> m_Exception;

		ShadingLanguageDefinitions m_Defs = {};

		uint32_t m_Line = 0;
	};
}
#pragma once

#include <ImTextEdit.h>

namespace LanguageDefinitions
{
	static const ImTextEdit::LanguageDefinition& GLSL()
	{
		static bool inited = false;
		static ImTextEdit::LanguageDefinition langDef;

		if (!inited)
		{
			static const char* const keywords[] = {
				"auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "inline", "int", "long", "register", "restrict", "return", "short",
				"signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while", "_Alignas", "_Alignof", "_Atomic", "_Bool", "_Complex", "_Generic", "_Imaginary",
				"_Noreturn", "_Static_assert", "_Thread_local", "attribute", "uniform", "varying", "layout", "centroid", "flat", "smooth", "noperspective", "patch", "sample", "subroutine", "in", "out", "inout",
				"bool", "true", "false", "invariant", "mat2", "mat3", "mat4", "dmat2", "dmat3", "dmat4", "mat2x2", "mat2x3", "mat2x4", "dmat2x2", "dmat2x3", "dmat2x4", "mat3x2", "mat3x3", "mat3x4", "dmat3x2", "dmat3x3", "dmat3x4",
				"mat4x2", "mat4x3", "mat4x4", "dmat4x2", "dmat4x3", "dmat4x4", "vec2", "vec3", "vec4", "ivec2", "ivec3", "ivec4", "bvec2", "bvec3", "bvec4", "dvec2", "dvec3", "dvec4", "uint", "uvec2", "uvec3", "uvec4",
				"lowp", "mediump", "highp", "precision", "sampler1D", "sampler2D", "sampler3D", "samplerCube", "sampler1DShadow", "sampler2DShadow", "samplerCubeShadow", "sampler1DArray", "sampler2DArray", "sampler1DArrayShadow",
				"sampler2DArrayShadow", "isampler1D", "isampler2D", "isampler3D", "isamplerCube", "isampler1DArray", "isampler2DArray", "usampler1D", "usampler2D", "usampler3D", "usamplerCube", "usampler1DArray", "usampler2DArray",
				"sampler2DRect", "sampler2DRectShadow", "isampler2DRect", "usampler2DRect", "samplerBuffer", "isamplerBuffer", "usamplerBuffer", "sampler2DMS", "isampler2DMS", "usampler2DMS", "sampler2DMSArray", "isampler2DMSArray",
				"usampler2DMSArray", "samplerCubeArray", "samplerCubeArrayShadow", "isamplerCubeArray", "usamplerCubeArray",
				"SHADERED_WEB", "SHADERED_DESKTOP", "SHADERED_VERSION", "shared", "writeonly", "readonly", "image2D", "image1D", "image3D"
			};

			for (auto& k : keywords)
				langDef.Keywords.insert(k);

			langDef.TokenRegexStrings.push_back(std::make_pair<std::string, ImTextEdit::PaletteIndex>("[ \\t]*#[ \\t]*[a-zA-Z_]+", ImTextEdit::PaletteIndex::Preprocessor));
			langDef.TokenRegexStrings.push_back(std::make_pair<std::string, ImTextEdit::PaletteIndex>("L?\\\"(\\\\.|[^\\\"])*\\\"", ImTextEdit::PaletteIndex::String));
			langDef.TokenRegexStrings.push_back(std::make_pair<std::string, ImTextEdit::PaletteIndex>("\\'\\\\?[^\\']\\'", ImTextEdit::PaletteIndex::CharLiteral));
			langDef.TokenRegexStrings.push_back(std::make_pair<std::string, ImTextEdit::PaletteIndex>("[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)([eE][+-]?[0-9]+)?[fF]?", ImTextEdit::PaletteIndex::Number));
			langDef.TokenRegexStrings.push_back(std::make_pair<std::string, ImTextEdit::PaletteIndex>("[+-]?[0-9]+[Uu]?[lL]?[lL]?", ImTextEdit::PaletteIndex::Number));
			langDef.TokenRegexStrings.push_back(std::make_pair<std::string, ImTextEdit::PaletteIndex>("0[0-7]+[Uu]?[lL]?[lL]?", ImTextEdit::PaletteIndex::Number));
			langDef.TokenRegexStrings.push_back(std::make_pair<std::string, ImTextEdit::PaletteIndex>("0[xX][0-9a-fA-F]+[uU]?[lL]?[lL]?", ImTextEdit::PaletteIndex::Number));
			langDef.TokenRegexStrings.push_back(std::make_pair<std::string, ImTextEdit::PaletteIndex>("[a-zA-Z_][a-zA-Z0-9_]*", ImTextEdit::PaletteIndex::Identifier));
			langDef.TokenRegexStrings.push_back(std::make_pair<std::string, ImTextEdit::PaletteIndex>("[\\[\\]\\{\\}\\!\\%\\^\\&\\*\\(\\)\\-\\+\\=\\~\\|\\<\\>\\?\\/\\;\\,\\.]", ImTextEdit::PaletteIndex::Punctuation));

			langDef.CommentStart = "/*";
			langDef.CommentEnd = "*/";
			langDef.SingleLineComment = "//";

			langDef.CaseSensitive = true;
			langDef.AutoIndentation = true;

			langDef.Name = "GLSL";

			inited = true;
		}

		return langDef;
	}
}
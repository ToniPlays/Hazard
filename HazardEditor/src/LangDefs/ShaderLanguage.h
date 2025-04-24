#pragma once

#include <ImTextEdit.h>

namespace LanguageDefinitions
{
	static const ImTextEdit::LanguageDefinition& HazardShader() 
	{
		static bool inited = false;
		static ImTextEdit::LanguageDefinition langDef;
		
		if(!inited)
		{
			static const char* const keywords[] = { "Shader", "Language", "Type", "Version", "Depth", "DepthWrite", "Properties", "Constants", 
				"Vertex", "Fragment", "Compute", "Include", "void", "struct", "Sampler2D", "SamplerCube", "Buffer", "flat", "float", "vec2",
				"vec3", "vec4", "int", "ivec2", "ivec3", "ivec4", "uint", "switch", "case", "default", "if", "else", "break", "discard"
			
			};
			
			for(auto& k : keywords)
				langDef.Keywords.insert(k);
			
			static const char* const identifiers[] = {
				"radians", "degrees", "sin", "cos", "tan", "asin", "acos", "atan", "sinh", "cosh", "tankh", "asinh", "acosh", "atanh", "pow", "exp", "log", "exp2", "log2", "sqrt", "inversesqrt",
				"abs", "sign", "floor", "trunc", "round", "roundEven", "ceil", "fract", "mod", "min", "max", "clamp", "mix", "step", "smoothstep", "isnan", "isinf", "floatBitsToint", "floatBitsToUint",
				"intBitsToFloat", "fma", "frexp", "Idexp", "length", "distance", "dot", "cross", "normalize", "faceforward", "reflect", "refract", "matrixCompMult", "outerProduct", "transpose",
				"determinant", "inverse", "all", "any", "not", "textureSize", "textureQueryLod", "textureQueryLevels", "textureSamples", "texture", "textureProj", "textureLod", "texelFetch", 
				"texelFetchOffset"
			};

			for (auto& k : identifiers)
			{
				ImTextEdit::Identifier id;
				id.Declaration = "Built-in function";
				langDef.Identifiers.insert(std::make_pair(std::string(k), id));
			}

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
			
			langDef.Name = "Shader";
			
			inited = true;
		}
		
		return langDef;
	}
}

#pragma once

#include <ImTextEdit.h>
#include <ImTextEdit.cpp>


namespace LanguageDefinitions
{
	static const ImTextEdit::LanguageDefinition& CPlusPlus()
	{
		static bool inited = false;
		static ImTextEdit::LanguageDefinition langDef;


		if (!inited)
		{
			static const char* const cppKeywords[] = {
				"alignas", "alignof", "and", "and_eq", "asm", "atomic_cancel", "atomic_commit", "atomic_noexcept", "auto", "bitand", "bitor", "bool", "break", "case", "catch", "char", "char16_t", "char32_t", "class",
				"compl", "concept", "const", "constexpr", "const_cast", "continue", "decltype", "default", "delete", "do", "double", "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false", "float",
				"for", "friend", "goto", "if", "import", "inline", "int", "long", "module", "mutable", "namespace", "new", "noexcept", "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private", "protected", "public",
				"register", "reinterpret_cast", "requires", "return", "short", "signed", "sizeof", "static", "static_assert", "static_cast", "struct", "switch", "synchronized", "template", "this", "thread_local",
				"throw", "true", "try", "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while", "xor", "xor_eq"
			};

			for (auto& k : cppKeywords)
				langDef.Keywords.insert(k);

			static const char* const identifiers[] = {
				"abort", "abs", "acos", "asin", "atan", "atexit", "atof", "atoi", "atol", "ceil", "clock", "cosh", "ctime", "div", "exit", "fabs", "floor", "fmod", "getchar", "getenv", "isalnum", "isalpha", "isdigit", "isgraph",
				"ispunct", "isspace", "isupper", "kbhit", "log10", "log2", "log", "memcmp", "modf", "pow", "printf", "sprintf", "snprintf", "putchar", "putenv", "puts", "rand", "remove", "rename", "sinh", "sqrt", "srand", "strcat", "strcmp", "strerror", "time", "tolower", "toupper",
				"std", "string", "vector", "map", "unordered_map", "set", "unordered_set", "min", "max"
			};

			for (auto& k : identifiers)
			{
				ImTextEdit::Identifier id;
				id.Declaration = "Built-in function";
				langDef.Identifiers.insert(std::make_pair(std::string(k), id));
			}

			langDef.Tokenize = [](const char* in_begin, const char* in_end, const char*& out_begin, const char*& out_end, ImTextEdit::PaletteIndex& paletteIndex) -> bool
				{
					paletteIndex = ImTextEdit::PaletteIndex::Max;

					while (in_begin < in_end && isascii(*in_begin) && isblank(*in_begin))
						in_begin++;

					if (in_begin == in_end)
					{
						out_begin = in_end;
						out_end = in_end;
						paletteIndex = ImTextEdit::PaletteIndex::Default;
					}
					else if (TokenizeCStyleString(in_begin, in_end, out_begin, out_end))
					{
						paletteIndex = ImTextEdit::PaletteIndex::String;
					}
					else if (TokenizeCStyleCharacterLiteral(in_begin, in_end, out_begin, out_end))
					{
						paletteIndex = ImTextEdit::PaletteIndex::CharLiteral;
					}
					else if (TokenizeCStyleIdentifier(in_begin, in_end, out_begin, out_end))
					{
						paletteIndex = ImTextEdit::PaletteIndex::Identifier;
					}
					else if (TokenizeCStyleNumber(in_begin, in_end, out_begin, out_end))
					{
						paletteIndex = ImTextEdit::PaletteIndex::Number;
					}
					else if (TokenizeCStylePunctuation(in_begin, in_end, out_begin, out_end))
					{
						paletteIndex = ImTextEdit::PaletteIndex::Punctuation;
					}

					return paletteIndex != ImTextEdit::PaletteIndex::Max;
				};

			langDef.CommentStart = "/*";
			langDef.CommentEnd = "*/";
			langDef.SingleLineComment = "//";

			langDef.CaseSensitive = true;
			langDef.AutoIndentation = true;

			langDef.Name = "C++";

			inited = true;
		}

		return langDef;
	}
}

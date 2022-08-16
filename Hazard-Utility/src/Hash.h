#pragma once

#include "UtilityCore.h"

class Hash {
public:
	static constexpr uint32_t GenerateFNVHash(const char* str)
	{
		constexpr uint32_t FNV_PRIME = 16777619u;
		constexpr uint32_t OFFSET_BASIS = 2166136261u;

		const size_t length = std::string_view(str).length() + 1;
		uint32_t hash = OFFSET_BASIS;
		for (size_t i = 0; i < length; ++i)
		{
			hash ^= *str++;
			hash *= FNV_PRIME;
		}
		return hash;
	}

	static constexpr uint32_t GenerateFNVHash(std::string_view string)
	{
		return GenerateFNVHash(string.data());
	}

	static uint32_t CRC32(const char* str);
	static uint32_t CRC32(const std::string& string);

};
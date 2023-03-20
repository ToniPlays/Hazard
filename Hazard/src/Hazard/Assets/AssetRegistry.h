#pragma once

#include "Asset.h"

#ifndef HZR_PLATFORM_WINDOWS
namespace std
{
	template <>
	struct hash<std::filesystem::path>
	{
		std::size_t operator()(const std::filesystem::path& path) const
		{
			return hash_value(path);
		}
	};
}
#endif

namespace Hazard
{
	class AssetRegistry
	{
	public:
		AssetRegistry() = default;
		~AssetRegistry() = default;

		bool Contains(const std::string& key) const;
		size_t Remove(const std::string& key);

		AssetMetadata& Get(const std::string& key);
		const AssetMetadata& Get(const std::string& key) const;
		std::unordered_map<std::string, AssetMetadata>& GetRegistry() { return m_Registry; }

		void Clear() { m_Registry.clear(); }

		AssetMetadata& operator[](const std::string& key);
		std::unordered_map<std::string, AssetMetadata>::iterator begin() { return m_Registry.begin(); }
		std::unordered_map<std::string, AssetMetadata>::iterator end() { return m_Registry.end(); }
		std::unordered_map<std::string, AssetMetadata>::const_iterator cbegin() { return m_Registry.cbegin(); }
		std::unordered_map<std::string, AssetMetadata>::const_iterator cend() { return m_Registry.cend(); }

	private:
		std::unordered_map<std::string, AssetMetadata> m_Registry;
	};
}

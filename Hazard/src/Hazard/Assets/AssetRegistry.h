#pragma once

#include "Asset.h"

#define APPLICATION_PERSISTENT_PATH "C:/dev/Hazard/HazardEditor"
/*
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
}*/

namespace Hazard
{
	class AssetRegistry
	{
	public:
		AssetRegistry() = default;
		~AssetRegistry() = default;

		bool Contains(const std::filesystem::path& path) const;
		size_t Remove(const std::filesystem::path& path);

		AssetMetadata& Get(const std::filesystem::path& path);
		const AssetMetadata& Get(const std::filesystem::path& path) const;
		std::unordered_map<std::filesystem::path, AssetMetadata>& GetRegistry() { return m_Registry; }

		void Clear() { m_Registry.clear(); }

		AssetMetadata& operator[](const std::filesystem::path& path);
		std::unordered_map<std::filesystem::path, AssetMetadata>::iterator begin() { return m_Registry.begin(); }
		std::unordered_map<std::filesystem::path, AssetMetadata>::iterator end() { return m_Registry.end(); }
		std::unordered_map<std::filesystem::path, AssetMetadata>::const_iterator cbegin() { return m_Registry.cbegin(); }
		std::unordered_map<std::filesystem::path, AssetMetadata>::const_iterator cend() { return m_Registry.cend(); }

	private:
		std::unordered_map<std::filesystem::path, AssetMetadata> m_Registry;
	};
}

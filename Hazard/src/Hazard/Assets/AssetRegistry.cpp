
#include <hzrpch.h>
#include "AssetRegistry.h"

namespace Hazard {

    static std::filesystem::path GetKey(const std::filesystem::path& key) {
        return key;
    }

    bool AssetRegistry::Contains(const std::filesystem::path& key) const
    {
        bool contains = m_Registry.find(key) != m_Registry.end();
        return contains;
    }

    size_t AssetRegistry::Remove(const const std::filesystem::path& key)
    {
        return m_Registry.erase(GetKey(key));
    }

    AssetMetadata& AssetRegistry::Get(const std::filesystem::path& key)
    {
        return m_Registry.at(GetKey(key));
    }

    const AssetMetadata& AssetRegistry::Get(const std::filesystem::path& key) const
    {
        return m_Registry.at(GetKey(key));
    }

    AssetMetadata& AssetRegistry::operator[](const std::filesystem::path& key)
    {
        return m_Registry[GetKey(key)];
    }
}

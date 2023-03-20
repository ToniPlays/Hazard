
#include <hzrpch.h>
#include "AssetRegistry.h"

namespace Hazard {

    static std::string GetKey(const std::string& key) {
        return key;
    }

    bool AssetRegistry::Contains(const std::string& key) const
    {
        bool contains = m_Registry.find(key) != m_Registry.end();
        return contains;
    }

    size_t AssetRegistry::Remove(const const std::string& key)
    {
        return m_Registry.erase(GetKey(key));
    }

    AssetMetadata& AssetRegistry::Get(const std::string& key)
    {
        return m_Registry.at(GetKey(key));
    }

    const AssetMetadata& AssetRegistry::Get(const std::string& key) const
    {
        return m_Registry.at(GetKey(key));
    }

    AssetMetadata& AssetRegistry::operator[](const std::string& key)
    {
        return m_Registry[GetKey(key)];
    }
}

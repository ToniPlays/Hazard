
#include <hzrpch.h>
#include "AssetRegistry.h"

namespace Hazard {

    static std::filesystem::path GetKey(const std::filesystem::path& path) {
        auto key = path;
        if (key.empty()) 
            key = path.lexically_normal();
        return key;
    }

    bool AssetRegistry::Contains(const std::filesystem::path& path) const
    {
        return m_Registry.find(GetKey(path)) != m_Registry.end();
    }

    size_t AssetRegistry::Remove(const std::filesystem::path& path) 
    {
        return m_Registry.erase(GetKey(path));
    }

    AssetMetadata& AssetRegistry::Get(const std::filesystem::path& path)
    {
        return m_Registry.at(GetKey(path));
    }

    const AssetMetadata& AssetRegistry::Get(const std::filesystem::path& path) const
    {
        return m_Registry.at(GetKey(path));
    }

    AssetMetadata& AssetRegistry::operator[](const std::filesystem::path& path)
    {
        HZR_CORE_ASSERT(!path.string().empty(), "File path empty!");
        return m_Registry[GetKey(path)];
    }
}

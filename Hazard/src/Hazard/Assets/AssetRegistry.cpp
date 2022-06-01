
#include <hzrpch.h>
#if 0
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
        std::filesystem::path key = GetKey(path);
        return m_Registry.find(key) != m_Registry.end();
    }

    size_t AssetRegistry::Remove(const std::filesystem::path& path) 
    {
        std::filesystem::path key = GetKey(path);
        return m_Registry.erase(key);
    }

    AssetMetadata& AssetRegistry::Get(const std::filesystem::path& path)
    {
        std::filesystem::path key = GetKey(path);
        return m_Registry.at(key);
    }

    const AssetMetadata& AssetRegistry::Get(const std::filesystem::path& path) const
    {
        std::filesystem::path key = GetKey(path);
        return m_Registry.at(key);
    }

    AssetMetadata& AssetRegistry::operator[](const std::filesystem::path& path)
    {
        HZR_CORE_ASSERT(!path.string().empty(), "File path empty!");
        std::filesystem::path key = GetKey(path);
        return m_Registry[key];
    }

}
#endif
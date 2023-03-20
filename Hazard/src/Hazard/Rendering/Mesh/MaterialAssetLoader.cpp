#include <hzrpch.h>
#include "MaterialAssetLoader.h"

#include "Hazard/Assets/AssetManager.h"

#include "Backend/Core/Utils.h"

#include "Material.h"

namespace Hazard
{
    Ref<JobGraph> MaterialAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
    {
        Ref<Material> material = Ref<Material>::Create();
        YAML::Node root = YAML::LoadFile(metadata.Path.string());
        
        for(auto& [name, texture] : material->GetTextures())
        {
            if(!root["Textures"][name]) continue;
            AssetHandle handle;
            YamlUtils::Deserialize(root["Textures"], name, handle, INVALID_ASSET_HANDLE);
            
            if(handle != INVALID_ASSET_HANDLE)
                material->SetTexture(name, AssetManager::GetAsset<Texture2DAsset>(handle));
        }
        
        for(auto& [name, param] : material->GetParameters())
        {
            if(!root["Properties"][name]) continue;
            
            HazardRenderer::Utils::Deserialize(root["Properties"][name], param.Offset, material->GetBuffer(), param.Type);
        }
        
        asset = material;
        return nullptr;
    }
    Ref<JobGraph> MaterialAssetLoader::Save(Ref<Asset>& asset)
    {
        YAML::Emitter out;
        
        Ref<Material> material = asset.As<Material>();
        out << YAML::BeginMap;
        out << YAML::Key << "Textures" << YAML::BeginMap;
        
        for(auto& [name, texture] : material->GetTextures())
            YamlUtils::Serialize(out, name, texture->GetHandle());
        
        out << YAML::EndMap;
        out << YAML::Key << "Properties" << YAML::BeginMap;
        
        for(auto& [name, param] : material->GetParameters())
        {
            void* value = material->GetValue(name);
            HazardRenderer::Utils::Serialize(out, name, value, param.Type);
        }
        
        out << YAML::EndMap << YAML::EndMap;
        
        AssetMetadata& data = AssetManager::GetMetadata(asset->GetHandle());
        File::WriteFile(data.Path, out.c_str());
        return nullptr;
    }
}


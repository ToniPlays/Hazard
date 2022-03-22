#include <hzrpch.h>
#include "Loaders.h"
#include "../Mesh/MeshFactory.h"
#include "../Mesh/MeshAsset.h"
#include "Hazard/Assets/AssetManager.h"
#include "../Texture.h"

namespace Hazard::Rendering 
{
    bool TextureLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
    {
        Texture2DCreateInfo info = {};
        info.FilePath = metadata.Path.string();
        //asset = Texture2D::Create(&info);
        return asset;
    }
    bool MeshLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
    {
        MeshCreateInfo meshCreateInfo = {};
        meshCreateInfo.Path = metadata.Path.string();
        
        Ref<Mesh> mesh = MeshFactory::LoadMesh(&meshCreateInfo);
        mesh->m_Type = ResourceType::Mesh;
        mesh->IncRefCount();

        asset = new MeshAsset();
        asset.As<MeshAsset>()->m_MeshSourceHandle = mesh->GetHandle();

        return asset;
    }
}

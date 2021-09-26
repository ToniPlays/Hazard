#include "hzrpch.h"
#include "Loaders.h"
#include "Texture/Texture2D.h"
#include "Mesh/MeshFactory.h"

namespace Hazard::Rendering 
{
    bool TextureLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
    {
        Texture2DCreateInfo info = { };
        info.filePath = metadata.Path.string();
        asset = Texture2D::Create(&info);
        return asset;
    }
    bool MeshLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
    {
        MeshCreateInfo meshCreateInfo = {};
        meshCreateInfo.Path = metadata.Path.string();

        asset = MeshFactory::LoadMesh(meshCreateInfo);
        return asset;
    }
}

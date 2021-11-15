#include "hzrpch.h"
#include "Loaders.h"
#include "../Mesh/MeshFactory.h"
#include "../Texture.h"

namespace Hazard::Rendering 
{
    bool TextureLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
    {
        Texture2DCreateInfo info = { };
        info.FilePath = metadata.Path.string();
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

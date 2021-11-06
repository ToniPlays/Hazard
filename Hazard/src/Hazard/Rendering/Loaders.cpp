#include "hzrpch.h"
#include "Loaders.h"
#include "Image/Image2D.h"
#include "Mesh/MeshFactory.h"

namespace Hazard::Rendering 
{
    bool ImageLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset)
    {
        Image2DCreateInfo info = { };
        info.FilePath = metadata.Path.string();
        asset = Image2D::Create(&info);
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

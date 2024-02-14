#pragma once
#include "Hazard/Assets/Asset.h"

class IAssetImporter
{
public:
	virtual void Init(AssetHandle handle) = 0;
	virtual void Init(const std::filesystem::path& sourcePath) = 0;
	virtual void RenderUI() = 0;
	virtual bool Import() = 0;
};
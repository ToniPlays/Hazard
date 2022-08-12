#pragma once

#include "UtilityCore.h"
#include "Hazard/Assets/Asset.h"

class EditorAssetManager 
{
public:
	EditorAssetManager() = default;
	~EditorAssetManager() = default;

	static Hazard::AssetMetadata ImportFromMetadata(const std::filesystem::path& path);
	static void CreateAsset(const AssetType& type, const std::filesystem::path& path);

	static bool CreateSourceAsset(const AssetType& type, const std::filesystem::path& path);
};
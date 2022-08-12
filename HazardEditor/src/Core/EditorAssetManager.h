#pragma once

#include "UtilityCore.h"
#include "Hazard/Assets/Asset.h"

struct ScriptCreateInfo {
	std::filesystem::path Path;
	std::string ClassName;
	std::string Derives;
	std::vector<std::string> Methods;
};

class EditorAssetManager
{
public:
	EditorAssetManager() = default;
	~EditorAssetManager() = default;

	static Hazard::AssetMetadata ImportFromMetadata(const std::filesystem::path& path);
	static bool CreateScriptAsset(const ScriptCreateInfo& info);
	static bool CreateFolder(const std::filesystem::path& path);
private:
	static bool CreateMetadataFile(const Hazard::AssetMetadata& metadata, const std::filesystem::path& path);
};
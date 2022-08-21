#pragma once

#include "UtilityCore.h"
#include "Hazard/Assets/Asset.h"

enum class MethodImpl 
{
	OnCreate,
	OnDestroy,
	OnUpdate,
	OnLateUpdate
};

struct ScriptCreateInfo 
{
	std::filesystem::path Path;
	std::string ClassName;
	std::string Derives;
	std::unordered_map<MethodImpl, std::string> Methods;

	void SetDefaults() {
		Path = "";
		ClassName = "";
		Derives = "Entity";
		Methods.clear();
	}
};

class EditorAssetManager
{
public:
	EditorAssetManager() = default;
	~EditorAssetManager() = default;

	static Hazard::AssetMetadata ImportFromMetadata(const std::filesystem::path& path);
	static bool CreateFolder(const std::filesystem::path& path);
	static bool CreateScriptAsset(const ScriptCreateInfo& info);
	static bool CreateWorld(const std::filesystem::path& path);
	static bool CreateMetadataFile(const Hazard::AssetMetadata& metadata, const std::filesystem::path& path);
	static bool RenameAsset(const std::string& newName, AssetHandle handle);
};
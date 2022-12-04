#pragma once

#include "UtilityCore.h"
#include "Hazard/Assets/Asset.h"
#include "Hazard/RenderContext/Texture2D.h"

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

	void SetDefaults() 
	{
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

	static void ImportAssets();
	static void Init();
	//TODO: Deprecate these
	static Hazard::AssetMetadata ImportFromMetadata(const std::filesystem::path& path);
	static bool CreateFolder(const std::filesystem::path& path);
	static bool CreateScriptAsset(const ScriptCreateInfo& info);
	static bool CreateWorld(const std::filesystem::path& path);
	static bool CreateMetadataFile(const Hazard::AssetMetadata& metadata, const std::filesystem::path& path);
	static bool RenameAsset(const std::string& newName, AssetHandle handle);
	//---------------------
	static Ref<Hazard::Texture2DAsset> GetIcon(const std::string& name);
	static void RefreshEditorAssets();

private:
	inline static std::unordered_map<std::string, Ref<Hazard::Texture2DAsset>> s_Icons;
};
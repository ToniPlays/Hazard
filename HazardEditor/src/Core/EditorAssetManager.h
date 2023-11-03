#pragma once

#include "UtilityCore.h"
#include "Hazard/Assets/Asset.h"
#include "Hazard/RenderContext/Texture2D.h"
#include "Hazard/Rendering/Mesh/Mesh.h"

#include "Hazard/Assets/AssetPack.h"
#include "Jobs.h"

enum class MethodImpl 
{
	OnCreate,
	OnDestroy,
	OnUpdate,
	OnLateUpdate
};

class EditorAssetManager
{
public:
	EditorAssetManager() = default;
	~EditorAssetManager() = default;

	static void Init();
	static void LoadEditorAssets();
    
	//---------------------
	static AssetHandle GetIconHandle(const std::string& name);
    static AssetHandle GetDefaultMesh(const std::string& name);
	static void SaveAssetsToPack(const std::string& name, std::vector<Hazard::AssetPackElement> assets);

private:
	static void ImportEngineAssets();

private:
	inline static std::unordered_map<std::string, AssetHandle> s_Icons;
    inline static std::unordered_map<std::string, AssetHandle> s_DefaultMesh;
};

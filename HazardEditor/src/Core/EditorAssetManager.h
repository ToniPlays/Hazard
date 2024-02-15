#pragma once

#include "UtilityCore.h"
#include "Hazard/Assets/Asset.h"
#include "Hazard/RenderContext/Texture2D.h"
#include "Hazard/Rendering/Mesh/Mesh.h"
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
	static void PostInit();
	static void LoadEditorAssets();
    
	//---------------------
	static AssetHandle GetIconHandle(const std::string& name);
    static AssetHandle GetDefaultMesh(const std::string& name);

private:
	static void ImportEngineShaders(std::vector<JobPromise>& promises);
	static void ImportEngineEnvironments(std::vector<JobPromise>& promises);
	static void ImportEngineMeshes(std::vector<JobPromise>& promises);
	static void ImportEngineImages(std::vector<JobPromise>& promises);


private:
	inline static std::unordered_map<std::string, AssetHandle> s_Icons;
    inline static std::unordered_map<std::string, AssetHandle> s_DefaultMesh;
};

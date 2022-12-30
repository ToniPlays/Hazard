
#include "EditorAssetManager.h"
#include "Hazard/Assets/AssetManager.h"
#include "File.h"
#include "Utility/YamlUtils.h"
#include "Hazard.h"
#include <Hazard/RenderContext/ShaderAsset.h>
#include <src/Core/GUIManager.h>
#include <src/GUI/Overlays/ProgressOverlay.h>

using namespace Hazard;


void EditorAssetManager::ImportAssets()
{
	for (auto& file : File::GetAllInDirectory("res/Icons", true))
		AssetManager::ImportAsset(file);
	for (auto& file : File::GetAllInDirectory("res/Mesh", true))
		AssetManager::ImportAsset(file);
	for (auto& file : File::GetAllInDirectory("res/Shaders", true))
		AssetManager::ImportAsset(file);
	for (auto& file : File::GetAllInDirectory("res/Textures", true))
		AssetManager::ImportAsset(file);
}
void EditorAssetManager::Init()
{
	struct EditorAsset
	{
		const char* Key;
		const char* Path;
	};

	std::vector<EditorAsset> texturesToLoad = {
		{ "Default", "res/Icons/textureBG.png"},
		{ "Folder", "res/Icons/folder.png"},
		{ "World", "res/Icons/world.png"},
		{ "Script", "res/Icons/csharp.png"},
		{ "Camera", "res/Icons/camera.png"},
		{ "DirectionalLight", "res/Icons/directionalLight.png"}
	};
    std::vector<EditorAsset> meshesToLoad = {
        { "Cube", "res/Mesh/cube.obj"     },
        { "Sphere", "res/Mesh/sphere.obj" }
    };
    
	std::vector<JobPromise> promises;
	promises.reserve(texturesToLoad.size());
	for (auto& texture : texturesToLoad)
	{
#if 0
		auto promise = AssetManager::GetAssetAsync<Texture2DAsset>(texture.Path);
		auto waitPromise = promise.Then(texture.Key, [texture](JobGraph& graph) -> size_t {
			s_Icons[texture.Key] = *graph.DependencyResult<Ref<Texture2DAsset>>();
			return 0;
			});

		promises.push_back(waitPromise);
#else
		s_Icons[texture.Key] = AssetManager::GetAsset<Texture2DAsset>(texture.Path);
#endif
	}
    
    for(auto& mesh : meshesToLoad)
        s_DefaultMesh[mesh.Key] = AssetManager::GetAsset<Mesh>(mesh.Path);
    
	for (auto& promise : promises)
		promise.Wait();

	RefreshEditorAssets();
}

AssetMetadata EditorAssetManager::ImportFromMetadata(const std::filesystem::path& path)
{
	AssetMetadata metadata = AssetMetadata();
	if (!File::Exists(path))
		return metadata;

	YAML::Node root = YAML::LoadFile(path.string());
	YamlUtils::Deserialize<AssetHandle>(root, "UID", metadata.Handle, INVALID_ASSET_HANDLE);
	YamlUtils::Deserialize<AssetType>(root, "Type", metadata.Type, AssetType::Undefined);
    metadata.Path = File::GetPathNoExt(path);
	Hazard::AssetManager::ImportAsset(File::GetPathNoExt(path), metadata);

	return metadata;
}

bool EditorAssetManager::CreateScriptAsset(const ScriptCreateInfo& info)
{
	HZR_ASSERT(!info.ClassName.empty(), "Class name cannot be empty");

	std::string methodList = "";

	for (auto& [key, method] : info.Methods) {
		methodList += method + "\n";
	}

	std::string sourceFile = File::ReadFile("res/ScriptTemplate/TemplateScript.cs");
	sourceFile = StringUtil::Replace(sourceFile, "%ScriptName%", info.ClassName);
	sourceFile = StringUtil::Replace(sourceFile, "%DerivesFrom%", info.Derives.empty() ? "" : ": " + info.Derives);
	sourceFile = StringUtil::Replace(sourceFile, "%MethodList%", methodList);

	if (!File::WriteFile(info.Path, sourceFile))
        return false;

	AssetMetadata metadata = {};
	metadata.Handle = AssetHandle();
	metadata.Path = info.Path;
	metadata.Type = AssetType::Script;

	return CreateMetadataFile(metadata, info.Path);
}

bool EditorAssetManager::CreateAsset(const AssetType& type, const std::filesystem::path& path)
{
    AssetMetadata metadata = {};
    metadata.Path = path;
    metadata.Type = type;
    
    switch(type)
    {
        case AssetType::World:
        {
            Ref<World> world = Ref<World>::Create(path);
            
            Entity camera = world->CreateEntity("Camera");
            camera.AddComponent<CameraComponent>();
            
            Entity cube = world->CreateEntity("Cube");
            //auto mesh = GetDefaultMesh("Cube");
            //cube.AddComponent<MeshComponent>().m_MeshHandle = mesh;
            
            metadata.Handle = AssetManager::NewAsset(world);
            AssetManager::ImportAsset(path, metadata);
            AssetManager::SaveAsset(world);
            break;
            
        }
        case AssetType::Shader:
        {
            std::string source = File::ReadFile("res/templates/shader.glsl");
            File::WriteFile(path, source);
            
            Ref<ShaderAsset> asset = Ref<ShaderAsset>::Create();
            metadata.Handle = AssetManager::NewAsset(asset);
            AssetManager::ImportAsset(path, metadata);
            AssetManager::SaveAsset(asset);
            break;
        }
        default: break;
    }
    
    if(metadata.Handle == INVALID_ASSET_HANDLE)
        return false;
    
    return CreateMetadataFile(metadata, path);
    
}

bool EditorAssetManager::CreateFolder(const std::filesystem::path& path)
{
	std::filesystem::path actualPath = path;
	if (File::DirectoryExists(actualPath))
    {
		size_t i = 1;
		std::filesystem::path curPath = File::AppendToName(actualPath, std::to_string(i));
		while (File::DirectoryExists(curPath))
		{
			curPath = File::AppendToName(actualPath, std::to_string(i));
			i++;
		}
		actualPath = curPath;
	}
	if (!File::CreateDir(actualPath)) return false;

	AssetMetadata metadata = {};
	metadata.Handle = AssetHandle();
	metadata.Path = actualPath;
	metadata.Type = AssetType::Folder;

	return CreateMetadataFile(metadata, actualPath);
}

bool EditorAssetManager::CreateMetadataFile(const AssetMetadata& metadata, const std::filesystem::path& path)
{
	std::filesystem::path metaPath = path.string() + ".meta";
	if (File::Exists(metaPath)) return false;
	YAML::Emitter out;

	out << YAML::BeginMap;
	YamlUtils::Serialize(out, "UID", metadata.Handle);
	YamlUtils::Serialize(out, "Type", metadata.Type);
	out << YAML::EndMap;
	File::WriteFile(metaPath, out.c_str());

	return Hazard::AssetManager::ImportAsset(path, metadata) != INVALID_ASSET_HANDLE;
}

bool EditorAssetManager::RenameAsset(const std::string& newName, AssetHandle handle)
{
	AssetMetadata& metadata = AssetManager::GetMetadata(handle);
    
	std::filesystem::path oldAssetPath = metadata.Path;
	std::string extension = File::GetFileExtension(oldAssetPath);
	std::filesystem::path newAssetPath;

	if (metadata.Type == AssetType::Folder)
	{
        File::RenameDirectory(oldAssetPath, newName);
        newAssetPath = oldAssetPath.parent_path() / newName;
	}
	else
	{
		newAssetPath = File::GetDirectoryOf(oldAssetPath) / (newName + "." + extension);
		File::WriteFile(newAssetPath.string());
		File::Move(oldAssetPath.string(), newAssetPath.string());
	}

	metadata.Path = newAssetPath;

	YAML::Emitter out;
	out << YAML::BeginMap;
	YamlUtils::Serialize(out, "UID", metadata.Handle);
	YamlUtils::Serialize(out, "Type", metadata.Type);
	out << YAML::EndMap;

	File::WriteFile(newAssetPath.string() + ".meta");
	HZR_ASSERT(File::Move(oldAssetPath.string() + ".meta", newAssetPath.string() + ".meta"), "Oops");
	File::WriteFile(newAssetPath.string() + ".meta", out.c_str());

	AssetManager::GetMetadataRegistry()[newAssetPath] = metadata;
	AssetManager::GetMetadataRegistry().erase(oldAssetPath);
    
	return true;
}
bool EditorAssetManager::MoveAssetToFolder(const AssetHandle& handle, const std::filesystem::path& path)
{
    AssetMetadata& data = AssetManager::GetMetadata(handle);
    std::filesystem::path newPath = path / File::GetName(data.Path);
    
    File::WriteFile(newPath);
    File::WriteFile(newPath.string() + ".meta");
    
    std::this_thread::sleep_for(500us);
    
    File::Move(data.Path, newPath);
    File::Move(data.Path.string() + ".meta", newPath.string() + ".meta");
    
    auto oldPath = data.Path;
    data.Path = newPath;
    AssetManager::GetMetadataRegistry()[newPath] = data;
    AssetManager::GetMetadataRegistry().erase(oldPath);
    
    return true;
}

Ref<Texture2DAsset> EditorAssetManager::GetIcon(const std::string& name)
{
	if (s_Icons.find(name) != s_Icons.end())
		return s_Icons[name];
	return s_Icons["Default"];
}
Ref<Mesh> EditorAssetManager::GetDefaultMesh(const std::string& name)
{
    if (s_DefaultMesh.find(name) != s_DefaultMesh.end())
        return s_DefaultMesh[name];
    return nullptr;;
}

void EditorAssetManager::RefreshEditorAssets(bool force)
{
	Timer timer;
	auto progressPanel = Application::GetModule<GUIManager>().GetPanelManager().GetRenderable<UI::ProgressOverlay>();

	//Compile non cached shaders
	for (auto& file : File::GetAllInDirectory("res/Shaders", true))
	{
		switch (Utils::AssetTypeFromExtension(File::GetFileExtension(file)))
		{
		case AssetType::Shader:
		{
			using namespace HazardRenderer;
			
			auto cacheDir = ShaderCompiler::GetCachedFilePath(file, RenderAPI::OpenGL);

			if (File::IsNewerThan(file, cacheDir) || force)
			{
				JobPromise promise = AssetManager::GetAssetAsync<Asset>(file, force ? AssetManagerFlags_ForceSource | AssetManagerFlags_ForceReload : 0);
				promise.Then([progressPanel](JobGraph& graph) -> size_t {
					Ref<Asset> asset = *graph.DependencyResult<Ref<Asset>>();
					JobPromise savePromise = AssetManager::SaveAssetAsync(asset);
					progressPanel->AddProcess("Caching shader", savePromise);
					return 0;
					});
				progressPanel->AddProcess("Loading shader", promise);
			}
			break;
		}
		default:
			break;
		}
	}

	HZR_INFO("Refresh editor assets in {0} ms", timer.ElapsedMillis());
}

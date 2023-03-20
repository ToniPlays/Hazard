
#include "EditorAssetManager.h"
#include "Hazard/Assets/AssetManager.h"
#include "File.h"
#include "Utility/YamlUtils.h"
#include "Hazard.h"
#include <Hazard/RenderContext/ShaderAsset.h>
#include <src/Core/GUIManager.h>
#include <src/GUI/Overlays/ProgressOverlay.h>

#include "Hazard/RenderContext/TextureFactory.h"

#include <FileCache.h>

using namespace Hazard;

void EditorAssetManager::Init()
{
	ImportEngineAssets();
}
void EditorAssetManager::LoadEditorAssets()
{
	struct EditorAsset
	{
		const char* Key;
		const char* Path;
		JobPromise<Ref<Asset>> Promise;
	};

	std::vector<EditorAsset> texturesToLoad = {
		{ "Default", "textureBG.hpack"},
		{ "Folder", "folder.hpack"},
		{ "World", "world.hpack"},
		{ "Script", "csharp.hpack"},
		{ "Camera", "camera.hpack"},
		{ "DirectionalLight", "directionalLight.hpack"}
	};
	std::vector<EditorAsset> meshesToLoad = {
		{ "Cube", "cube.hpack"     },
		{ "Sphere", "sphere.hpack" }
	};

	Timer timer;
	for (auto& texture : texturesToLoad)
		texture.Promise = AssetManager::GetAssetAsync<Asset>(texture.Path);

	for (auto& mesh : meshesToLoad)
		mesh.Promise = AssetManager::GetAssetAsync<Asset>(mesh.Path);

	for (auto& texture : texturesToLoad)
	{
		texture.Promise.Wait();
		s_Icons[texture.Key] = texture.Promise.Get().As<Texture2DAsset>();
	}
	for (auto& mesh : meshesToLoad)
	{
		mesh.Promise.Wait();
		s_DefaultMesh[mesh.Key] = mesh.Promise.Get().As<Mesh>();
	}

	HZR_INFO("Editor assets loaded in {}ms", timer.ElapsedMillis());
}

AssetMetadata EditorAssetManager::ImportFromMetadata(const std::filesystem::path& path)
{
	AssetMetadata metadata = AssetMetadata();
	if (!File::Exists(path))
		return metadata;

	YAML::Node root = YAML::LoadFile(path.string());
	YamlUtils::Deserialize<AssetHandle>(root, "UID", metadata.Handle, INVALID_ASSET_HANDLE);
	YamlUtils::Deserialize<AssetType>(root, "Type", metadata.Type, AssetType::Undefined);
	//metadata.Path = File::GetPathNoExt(path);
	//Hazard::AssetManager::ImportAsset(File::GetPathNoExt(path), metadata);

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
	//metadata.Path = info.Path;
	metadata.Type = AssetType::Script;

	return CreateMetadataFile(metadata, info.Path);
}

bool EditorAssetManager::CreateAsset(const AssetType& type, const std::filesystem::path& path)
{
	AssetMetadata metadata = {};
	//metadata.Path = path;
	metadata.Type = type;

	switch (type)
	{
		/*
		case AssetType::World:
		{
			Ref<World> world = Ref<World>::Create(path);

			Entity camera = world->CreateEntity("Camera");
			camera.AddComponent<CameraComponent>();

			world->CreateEntity("Cube").AddComponent<MeshComponent>();

			metadata.Handle = AssetManager::NewAsset(world);
			AssetManager::ImportAsset(path, metadata);
			AssetManager::SaveAsset(world);
			break;

		}
		case AssetType::Material:
		{
			Ref<Material> material = Ref<Material>::Create();
			metadata.Handle = AssetManager::NewAsset(material);
			AssetManager::ImportAsset(path, metadata);
			AssetManager::SaveAsset(material);
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
		*/
	default: break;
	}

	if (metadata.Handle == INVALID_ASSET_HANDLE)
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
	//metadata.Path = actualPath;
	metadata.Type = AssetType::Folder;

	return CreateMetadataFile(metadata, actualPath);
}

bool EditorAssetManager::CreateMetadataFile(const AssetMetadata& metadata, const std::filesystem::path& path)
{
	return false; // Hazard::AssetManager::ImportAsset(path, metadata) != INVALID_ASSET_HANDLE;
}

bool EditorAssetManager::RenameAsset(const std::string& newName, AssetHandle handle)
{
	AssetMetadata& metadata = AssetManager::GetMetadata(handle);

	std::filesystem::path oldAssetPath = metadata.Key;
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

	metadata.Key = newAssetPath.string();

	YAML::Emitter out;
	out << YAML::BeginMap;
	YamlUtils::Serialize(out, "UID", metadata.Handle);
	YamlUtils::Serialize(out, "Type", metadata.Type);
	out << YAML::EndMap;

	File::WriteFile(newAssetPath.string() + ".meta");
	HZR_ASSERT(File::Move(oldAssetPath.string() + ".meta", newAssetPath.string() + ".meta"), "Oops");
	File::WriteFile(newAssetPath.string() + ".meta", out.c_str());

	//AssetManager::GetMetadataRegistry()[newAssetPath] = metadata;
	//AssetManager::GetMetadataRegistry().erase(oldAssetPath);

	return true;
}
bool EditorAssetManager::MoveAssetToFolder(const AssetHandle& handle, const std::filesystem::path& path)
{
	AssetMetadata& data = AssetManager::GetMetadata(handle);
	std::filesystem::path newPath = path / File::GetName(data.Key);

	if (File::Exists(newPath))
		return false;

	File::WriteFile(newPath);
	File::WriteFile(newPath.string() + ".meta");

	std::this_thread::sleep_for(500us);

	//File::Move(data.Path, newPath);
	//File::Move(data.Path.string() + ".meta", newPath.string() + ".meta");

	//auto oldPath = data.Path;
	//data.Path = newPath;

	//AssetManager::GetMetadataRegistry()[newPath] = data;
	//AssetManager::GetMetadataRegistry().erase(oldPath);

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
	return nullptr;
}

void EditorAssetManager::GenerateAndSavePack(Ref<Job> job, std::filesystem::path& path)
{
	FileCache cache("Library");

	auto packPath = File::GetNameNoExt(path) + ".hpack";
	CachedBuffer buffer = EditorAssetManager::GenerateAssetPack(path);
	if (buffer.GetSize() == 0) return;

	if (!File::WriteBinaryFile(cache.GetCachePath() / packPath, buffer.GetData(), buffer.GetSize()))
		HZR_WARN("Asset pack saving failed");
}

void EditorAssetManager::ImportEngineAssets()
{
	using namespace Hazard;
	Timer timer;
	HZR_INFO("Importing engine assets");

	FileCache cache("Library");

	std::vector<Ref<Job>> jobs;

	for (auto& file : File::GetAllInDirectory("res", true))
	{
		auto packPath = File::GetNameNoExt(file) + ".hpack";
		if (cache.HasFile(packPath)) continue;

		Ref<Job> job = Ref<Job>::Create(GenerateAndSavePack, file);
		jobs.push_back(job);
	}

	Ref<JobGraph> loadingGraph = Ref<JobGraph>::Create("EngineLoad", 1);
	loadingGraph->GetStage(0)->QueueJobs(jobs);

	JobSystem& system = Application::Get().GetJobSystem();
	JobPromise<bool> promise = system.QueueGraph<bool>(loadingGraph);
	promise.Wait();
	

	for (auto& file : File::GetAllInDirectory(cache.GetCachePath(), true))
	{
		auto packPath = File::GetNameNoExt(file) + ".hpack";
		CachedBuffer buffer = File::ReadBinaryFile(file);

		AssetPack pack = AssetPack::Create(buffer, file);

		for (auto& element : pack.Elements)
			AssetManager::ImportAsset(element, File::GetName(packPath));
	}
	HZR_INFO("Engine assets imported in {}ms", timer.ElapsedMillis());
}

CachedBuffer EditorAssetManager::GenerateAssetPack(const std::filesystem::path& path)
{
	AssetType type = Hazard::Utils::AssetTypeFromExtension(File::GetFileExtension(path));
	if (type == AssetType::Undefined || type == AssetType::Folder)
		return CachedBuffer();

	AssetPackHeader header;
	header.ElementCount = 1;

	std::vector<AssetPackElementHeader> elements;
	std::vector<Buffer> dataBuffers;

	size_t dataOffset = 0;

	switch (type)
	{
	case AssetType::Shader:
	{
		std::vector<ShaderStageCode> binaries = ShaderCompiler::GetShaderBinariesFromSource(path, RenderAPI::Vulkan);

		size_t codeSize = 0;
		for (auto& stage : binaries)
			codeSize += sizeof(ShaderStage) + sizeof(uint32_t) + stage.ShaderCode.Size;

		AssetPackElementHeader& element = elements.emplace_back();
		element.Type = (uint32_t)type;
		element.AssetDataSize = codeSize;
		element.AssetDataOffset = dataOffset;
		element.Handle = AssetHandle();

		Buffer& data = dataBuffers.emplace_back();
		data.Allocate(sizeof(uint32_t) * 2 + binaries.size() * codeSize);

		size_t offset = 0;

		for (auto& stage : binaries)
		{
			data.Write(&stage.Stage, sizeof(uint32_t), offset);
			offset += sizeof(uint32_t);
			data.Write(&stage.Size, sizeof(uint32_t), offset);
			offset += sizeof(uint32_t);
			data.Write(stage.ShaderCode.Data, stage.ShaderCode.Size, offset);
			offset += stage.ShaderCode.Size;
			
		}
		dataOffset += offset;
		
		break;
	}
	case AssetType::Image:
	{
		TextureHeader header = TextureFactory::LoadTextureFromSourceFile(path, true);


		AssetPackElementHeader& element = elements.emplace_back();
		element.Type = (uint32_t)type;
		element.AssetDataSize = sizeof(TextureFileHeader) + header.Width * header.Height * header.Channels;
		element.AssetDataOffset = dataOffset;
		element.Handle = AssetHandle();

		Buffer& data = dataBuffers.emplace_back();
		data.Allocate(element.AssetDataSize);

		TextureFileHeader fileHeader = {};
		fileHeader.Width = header.Width;
		fileHeader.Height = header.Height;
		fileHeader.Dimensions = header.Dimensions;
		fileHeader.Channels = header.Channels;
		fileHeader.Format = header.Format;

		data.Write(&fileHeader, sizeof(TextureFileHeader));
		data.Write(header.ImageData.Data, header.ImageData.Size, sizeof(TextureFileHeader));

		break;
	}
	}
	size_t bufferSize = sizeof(AssetPackHeader) + elements.size() * sizeof(AssetPackElementHeader);

	for (auto& buffer : dataBuffers)
		bufferSize += buffer.Size;

	CachedBuffer buffer(bufferSize);
	buffer.Write(header);
	buffer.Write(elements.data(), elements.size() * sizeof(AssetPackElementHeader));

	for (auto& data : dataBuffers)
		buffer.Write(data);

	return buffer;
}

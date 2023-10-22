#include "EditorAssetPackBuilder.h"

#include "Hazard/Assets/AssetManager.h"
#include "GUIManager.h"
#include <GUI/ProjectPanel/AssetPanel.h>

AssetPack EditorAssetPackBuilder::CreateAssetPack(const std::vector<AssetPackElement> element)
{
	AssetPack pack = {};
	pack.Handle = AssetHandle();
	pack.ElementCount = element.size();
	pack.Elements = element;

	for (auto& e : pack.Elements)
		e.AssetPackHandle = pack.Handle;

	return pack;
}

void EditorAssetPackBuilder::GenerateAndSaveAssetPack(JobInfo& info, const std::filesystem::path& path)
{
	Ref<GraphStage> previousStage = info.Job->GetJobGraph()->GetPreviousStage();

	auto results = previousStage->GetJobResults();

	std::vector<AssetPackElement> elements;
	elements.reserve(results.size());

	for (auto& result : results)
		elements.push_back(result.Read<AssetPackElement>());

	AssetPack pack = EditorAssetPackBuilder::CreateAssetPack(elements);
	CachedBuffer buffer = AssetPack::ToBuffer(pack);

	//TODO: FIX ME
	File::WriteBinaryFile(path, buffer.GetData(), buffer.GetSize());
	AssetManager::ImportAssetPack(pack, path);

	pack.Free();
}


void EditorAssetPackBuilder::ImageAssetPackJob(JobInfo& info, const std::filesystem::path& file, HazardRenderer::Image2DCreateInfo imageInfo, UI::ImageImportSettings settings)
{
	using namespace HazardRenderer;

	if (!File::Exists(file))
		throw JobException(fmt::format("Source file does not exist: {}", file.string()));

	TextureHeader textureHeader = TextureFactory::LoadTextureFromSourceFile(file, settings.FlipOnLoad);

	Buffer data;
	data.Allocate(sizeof(TextureFileHeader) + textureHeader.Width * textureHeader.Height * textureHeader.Channels);

	TextureFileHeader fileHeader = {};
	fileHeader.Width = textureHeader.Width;
	fileHeader.Height = textureHeader.Height;
	fileHeader.Dimensions = textureHeader.Dimensions;
	fileHeader.Channels = textureHeader.Channels;
	fileHeader.Format = textureHeader.Format;
	fileHeader.MinFilter = (uint8_t)settings.MinFilter;
	fileHeader.MagFilter = (uint8_t)settings.MagFilter;
	fileHeader.WrapMode = (uint8_t)settings.Wrapping;

	data.Write(&fileHeader, sizeof(TextureFileHeader));
	data.Write(textureHeader.ImageData.Data, textureHeader.ImageData.Size, sizeof(TextureFileHeader));

	AssetPackElement element = {};
	element.Type = AssetType::Image;
	element.AssetPackHandle = INVALID_ASSET_HANDLE;
	element.Handle = settings.Handle;
	element.AddressableName = File::GetName(file);
	element.Data = data;

	info.Job->SetResult(&element, sizeof(AssetPackElement));
	textureHeader.ImageData.Release();
}

void EditorAssetPackBuilder::MeshAssetPackJob(JobInfo& info, const std::filesystem::path& file, MeshCreateInfo meshInfo, UI::MeshImportSettings settings)
{
	MeshFactory factory;
	factory.SetOptimization(MeshLoaderFlags_DefaultFlags);
	factory.SetScalar(settings.Scale);
	factory.SetProgressHandler([&](float progress) {
		info.Job->Progress(progress * 0.9f);
	});

	MeshData meshData = factory.LoadMeshFromSource(file);

	MeshFileHeader header;
	header.Flags = meshData.Flags;
	header.VertexCount = meshData.Vertices.size();
	header.IndexCount = meshData.Indices.size();
	header.BoundingBox = meshData.BoundingBox;

	Buffer data;
	uint32_t writeOffset = 0;

	data.Allocate(sizeof(MeshFileHeader) + factory.GetMeshDataSize(meshData));
	data.Write(&header, sizeof(MeshFileHeader));
	writeOffset += sizeof(MeshFileHeader);

	for (uint64_t i = 0; i < meshData.Vertices.size(); i++)
	{
		Vertex3D& v = meshData.Vertices[i];

		if (meshData.Flags & MeshFlags_Positions)
		{
			data.Write(&v.Position, sizeof(glm::vec3), writeOffset);
			writeOffset += sizeof(glm::vec3);
		}
		if (meshData.Flags & MeshFlags_VertexColors)
		{
			data.Write(&v.Color, sizeof(glm::vec4), writeOffset);
			writeOffset += sizeof(glm::vec4);
		}
		if (meshData.Flags & MeshFlags_Normals)
		{
			data.Write(&v.Normals, sizeof(glm::vec3), writeOffset);
			writeOffset += sizeof(glm::vec3);
		}
		if (meshData.Flags & MeshFlags_Tangent)
		{
			data.Write(&v.Tangent, sizeof(glm::vec3), writeOffset);
			writeOffset += sizeof(glm::vec3);
		}
		if (meshData.Flags & MeshFlags_Binormal)
		{
			data.Write(&v.Binormal, sizeof(glm::vec3), writeOffset);
			writeOffset += sizeof(glm::vec3);
		}
		if (meshData.Flags & MeshFlags_TextCoord)
		{
			data.Write(&v.Position, sizeof(glm::vec2), writeOffset);
			writeOffset += sizeof(glm::vec2);
		}
		info.Job->Progress(0.9f + ((float)i / (float)meshData.Vertices.size()) * 0.1f);
	}
	data.Write(meshData.Indices.data(), meshData.Indices.size() * sizeof(uint32_t), writeOffset);

	AssetPackElement result = {};
	result.Handle = settings.Handle;
	result.Type = AssetType::Mesh;
	result.Data = data;
	result.AddressableName = File::GetName(file);

	info.Job->SetResult(&result, sizeof(AssetPackElement));
}

void EditorAssetPackBuilder::ShaderAssetPackJob(JobInfo& info, const std::filesystem::path& file, HazardRenderer::RenderAPI api)
{
	using namespace HazardRenderer;
	std::vector<ShaderStageCode> binaries = ShaderCompiler::GetShaderBinariesFromSource(file, api);
    
	uint64_t codeSize = 0;
	for (auto& stage : binaries)
		codeSize += sizeof(ShaderStageFlags) + sizeof(uint32_t) + stage.ShaderCode.Size;

	Buffer data;
	//Header and combined binary size
	data.Allocate(codeSize);

	uint64_t offset = 0;

	for (auto& stage : binaries)
	{
		data.Write(&stage.Stage, sizeof(uint32_t), offset);
		offset += sizeof(uint32_t);
		data.Write(&stage.Size, sizeof(uint32_t), offset);
		offset += sizeof(uint32_t);
		data.Write(stage.ShaderCode.Data, stage.ShaderCode.Size, offset);
		offset += stage.ShaderCode.Size;
	}

	AssetPackElement result = {};
	result.Type = AssetType::Shader;
	result.Data = data;
	result.Handle = AssetHandle();
	result.AddressableName = File::GetName(file);

	info.Job->SetResult(&result, sizeof(AssetPackElement));
}

void EditorAssetPackBuilder::MaterialAssetPackJob(JobInfo& info, const std::filesystem::path& file, UI::MaterialImportSettings settings)
{
	MaterialAssetHeader header = {};

	header.PipelineHandle = settings.PipelineHandle;
	header.TextureCount = 0;
	header.ParameterCount = 0;

	Buffer data;
	data.Allocate(sizeof(MaterialAssetHeader));
	data.Write(&header, sizeof(MaterialAssetHeader));

	AssetPackElement result = {};
	result.Type = AssetType::Material;
	result.Data = data;
	result.Handle = settings.Handle;
	result.AddressableName = File::GetName(file);

	info.Job->SetResult(&result, sizeof(AssetPackElement));
}

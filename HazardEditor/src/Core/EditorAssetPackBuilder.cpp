#include "EditorAssetPackBuilder.h"

#include "Hazard/Assets/AssetManager.h"

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
CachedBuffer EditorAssetPackBuilder::AssetPackToBuffer(const AssetPack& pack)
{
	AssetPackHeader header = {};
	header.Handle = pack.Handle;
	header.ElementCount = pack.ElementCount;

	size_t dataSize = 0;

	for (auto& element : pack.Elements)
		dataSize += element.Data.Size;

	CachedBuffer buffer(sizeof(AssetPackHeader) + sizeof(AssetPackElementHeader) * pack.ElementCount + dataSize);

	uint32_t dataOffset = 0;

	buffer.Write(header);

	//Write headers
	for (auto& element : pack.Elements)
	{
		AssetPackElementHeader elementHeader = {};
		elementHeader.Type = (uint32_t)element.Type;
		elementHeader.AssetDataSize = element.Data.Size;
		elementHeader.AssetDataOffset = dataOffset;
		elementHeader.Handle = element.Handle;

		buffer.Write(elementHeader);

		dataOffset += element.Data.Size;
	}
	//Write actual asset data
	for (auto& element : pack.Elements)
		buffer.Write(element.Data);

	return buffer;
}

void EditorAssetPackBuilder::GenerateAndSaveAssetPack(Ref<Job> job, const std::filesystem::path& path)
{
	AssetPackElement element = job->GetInput<AssetPackElement>();

	std::vector<AssetPackElement> elements = { element };
	AssetPack pack = EditorAssetPackBuilder::CreateAssetPack(elements);
	CachedBuffer buffer = EditorAssetPackBuilder::AssetPackToBuffer(pack);

	//Save asset pack and generate meta file

	File::WriteBinaryFile(path, buffer.GetData(), buffer.GetSize());

	AssetManager::ImportAssetPack(pack, path);

	pack.Free();
}


void EditorAssetPackBuilder::ImageAssetPackJob(Ref<Job> job, const std::filesystem::path& file, HazardRenderer::Image2DCreateInfo info, UI::ImageImportSettings settings)
{
	TextureHeader textureHeader = TextureFactory::LoadTextureFromSourceFile(file, settings.FlipOnLoad);

	Buffer data;
	data.Allocate(sizeof(TextureFileHeader) + textureHeader.Width * textureHeader.Height * textureHeader.Channels);

	TextureFileHeader fileHeader = {};
	fileHeader.Width = textureHeader.Width;
	fileHeader.Height = textureHeader.Height;
	fileHeader.Dimensions = textureHeader.Dimensions;
	fileHeader.Channels = textureHeader.Channels;
	fileHeader.Format = textureHeader.Format;
	fileHeader.MinFilter = (uint8_t)info.Filters.MinFilter;
	fileHeader.MagFilter = (uint8_t)info.Filters.MagFilter;
	fileHeader.WrapMode = (uint8_t)info.Filters.Wrapping;

	data.Write(&fileHeader, sizeof(TextureFileHeader));
	data.Write(textureHeader.ImageData.Data, textureHeader.ImageData.Size, sizeof(TextureFileHeader));

	AssetPackElement element = {};
	element.Type = AssetType::Image;
	element.AssetPackHandle = INVALID_ASSET_HANDLE;
	element.Handle = settings.Handle;
	element.Data = data;

	job->GetStage()->SetResult(element);

	textureHeader.ImageData.Release();
}

void EditorAssetPackBuilder::MeshAssetPackJob(Ref<Job> job, const std::filesystem::path& file, MeshCreateInfo info, UI::MeshImportSettings settings)
{
	job->SetJobName(File::GetName(file));
	MeshFactory factory;
	factory.SetOptimization(MeshLoaderFlags_DefaultFlags);
	factory.SetScalar(settings.Scale);
	factory.SetProgressHandler([&](float progress) {
		job->Progress(progress * 0.9f);
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

	for (size_t i = 0; i < meshData.Vertices.size(); i++)
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
		job->Progress(0.9f + ((float)i / (float)meshData.Vertices.size()) * 0.1f);
	}
	data.Write(meshData.Indices.data(), meshData.Indices.size() * sizeof(uint32_t), writeOffset);

	AssetPackElement result = {};
	result.Handle = settings.Handle;
	result.Type = AssetType::Mesh;
	result.Data = data;

	job->GetStage()->SetResult(result);
}

void EditorAssetPackBuilder::ShaderAssetPackJob(Ref<Job> job, const std::filesystem::path& file, HazardRenderer::RenderAPI api)
{
	using namespace HazardRenderer;
	std::vector<ShaderStageCode> binaries = ShaderCompiler::GetShaderBinariesFromSource(file, api);

	size_t codeSize = 0;
	for (auto& stage : binaries)
		codeSize += sizeof(ShaderStage) + sizeof(uint32_t) + stage.ShaderCode.Size;

	Buffer data;
	//Header and combined binary size
	data.Allocate(codeSize);

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

	AssetPackElement result = {};
	result.Type = AssetType::Shader;
	result.Data = data;
	result.Handle = AssetHandle();

	job->GetStage()->SetResult(result);
}

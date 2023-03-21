#pragma once

#include "Hazard/Assets/AssetPack.h"
#include "Hazard/RenderContext/TextureFactory.h"
#include "Hazard/Rendering/Mesh/MeshFactory.h"
#include "HazardRenderer.h"
#include <Hazard/Rendering/Mesh/MeshAssetLoader.h>

using namespace Hazard;
class EditorAssetPackBuilder
{
public:

	static AssetPack CreateAssetPack(const std::vector<AssetPackElement> element);
	static CachedBuffer AssetPackToBuffer(const AssetPack& pack);

	template<typename T>
	static AssetPackElement CreatePackElement(const std::filesystem::path& file, T info)
	{
		__debugbreak();
		return AssetPackElement();
	}
	template<typename T, typename E>
	static AssetPackElement CreatePackElement(const std::filesystem::path& file, T info, E setting)
	{
		__debugbreak();	
		return AssetPackElement();
	}
	template<>
	static AssetPackElement CreatePackElement(const std::filesystem::path& file, HazardRenderer::Image2DCreateInfo info, bool flipOnLoad)
	{
		TextureHeader textureHeader = TextureFactory::LoadTextureFromSourceFile(file, flipOnLoad);

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
		element.Data = data;

		textureHeader.ImageData.Release();

		return element;
	}
	template<>
	static AssetPackElement CreatePackElement(const std::filesystem::path& file, HazardRenderer::RenderAPI renderApi)
	{
		if (!File::Exists(file)) return AssetPackElement();

		using namespace HazardRenderer;
		std::vector<ShaderStageCode> binaries = ShaderCompiler::GetShaderBinariesFromSource(file, renderApi);

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

		return result;
	}
	template<>
	static AssetPackElement CreatePackElement(const std::filesystem::path& file, MeshCreateInfo info)
	{
		if (!File::Exists(file)) return AssetPackElement();
	
		MeshFactory factory;
		factory.SetOptimization(MeshLoaderFlags_DefaultFlags);
		factory.SetScalar(1.0f);
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
		
		for (auto& v : meshData.Vertices)
		{
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
		}
		data.Write(meshData.Indices.data(), meshData.Indices.size() * sizeof(uint32_t), writeOffset);

		AssetPackElement result = {};
		result.Type = AssetType::Mesh;
		result.Data = data;

		return result;
	}
};
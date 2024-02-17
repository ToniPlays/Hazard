#pragma once

#include "Hazard/Core/Core.h"
#include "HazardRendererCore.h"
#include "Core/Rendering/ShaderDataType.h"

#include "BoundingBox.h"
#include <Hazard/Rendering/Vertices.h>

namespace Hazard
{
	class MeshImporter : public RefCount
	{
	public:

		enum class TextureType
		{
			Albedo,
			Diffuse,
			Specular,
			Emission,
			Metalness,
			Normal,
			Height,
			Shininess,
			Displacement,
			Lightmap,
			Reflection
		};

		struct SceneMetadata
		{
			uint32_t AnimationCount;
			uint32_t CameraCount;
			uint32_t LightCount;
			uint32_t MeshCount;
			uint32_t MaterialCount;
			uint32_t TextureCount;
		};

		struct TextureMetadata
		{
			std::string Name;
			uint32_t TextureIndex;
		};

		struct TextureData
		{
			std::string Name;
			uint32_t Width;
			uint32_t Height;
			Buffer ImageData;
		};

		struct MaterialMetadata
		{
			std::string Name;
			uint32_t MaterialIndex;
			uint32_t PropertyCount;
			std::unordered_map<TextureType, uint32_t> Textures;
		};

		struct MaterialProperty
		{
			std::string Name;
			TextureType Usage;
			HazardRenderer::ShaderDataType Type;
			Buffer Data;
		};

		struct MaterialData
		{
			std::string Name;
			std::vector<MaterialProperty> Properties;
		};

		struct MeshMetadata
		{
			std::string Name;
			uint32_t MeshIndex;
			uint32_t MaterialIndex;
			uint64_t VertexCount;
			uint64_t IndexCount;
			uint32_t BoneCount;
			uint32_t AnimatedMeshCount;
		};

		struct MeshData
		{
			std::string Name;
			glm::mat4 Transform;
			uint64_t MaterialIndex;
			BoundingBox BoundingBox;
			std::vector<Vertex3D> Vertices;
			std::vector<uint32_t> Indices;
		};

		struct AnimationData
		{
			std::string Name;
			float Duration;
		};

		virtual SceneMetadata GetSceneMetadata() = 0;
		virtual std::vector<MeshImporter::TextureMetadata> GetTextures() = 0;
		virtual std::vector<MaterialMetadata> GetMaterials() = 0;
		virtual std::vector<MeshMetadata> GetMeshes() = 0;
		virtual std::vector<AnimationData> GetAnimations() = 0;
		virtual TextureData GetTextureData(uint32_t textureIndex) = 0;
		virtual MeshData GetMeshData(const MeshMetadata& mesh, const std::function<void(uint32_t, uint32_t)>& progress = nullptr) = 0;
		virtual MaterialData GetMaterial(uint32_t materialIndex) = 0;
		virtual glm::mat4 GetMeshTransform(uint32_t meshIndex) = 0;

		virtual void AddImportProgressCallback(const std::function<void(float)> callback) = 0;

	};
}
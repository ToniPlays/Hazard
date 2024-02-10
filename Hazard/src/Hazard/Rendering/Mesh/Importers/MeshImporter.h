#pragma once

#include "Hazard/Core/Core.h"
#include "HazardRendererCore.h"

#include "BoundingBox.h"
#include <Hazard/Rendering/Vertices.h>

namespace Hazard
{
	class MeshImporter : public RefCount
	{
	public:

		struct SceneMetadata
		{
			uint32_t AnimationCount;
			uint32_t CameraCount;
			uint32_t LightCount;
			uint32_t MeshCount;
			uint32_t MaterialCount;
			uint32_t TextureCount;
		};

		struct TextureData
		{
			std::string Filename;
			Buffer TextureData;
		};

		struct MeshMetadata
		{
			std::string Name;
			uint32_t MaterialIndex;
			uint64_t VertexCount;
			uint64_t IndexCount;
			uint32_t BoneCount;
			uint32_t AnimatedMeshCount;
			BoundingBox BoundingBox;
		};

		struct MeshData
		{
			std::string Name;
			std::vector<Vertex3D> Vertices;
			std::vector<uint32_t> Indices;
		};

		struct AnimationData
		{
			std::string Name;
			float Duration;
		};

		virtual SceneMetadata GetSceneMetadata() = 0;
		virtual std::vector<TextureData> GetTextures() = 0;
		virtual std::vector<MeshMetadata> GetMeshes() = 0;
		virtual MeshData GetMeshData(uint32_t meshIndex, const std::function<void(uint32_t, uint32_t)>& progress = nullptr) = 0;
		virtual std::vector<AnimationData> GetAnimations() = 0;

		virtual glm::mat4 GetMeshTransform(uint32_t meshIndex) = 0;
	};
}
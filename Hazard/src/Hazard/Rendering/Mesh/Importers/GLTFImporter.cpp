#include "GLTFImporter.h"

namespace Hazard 
{
	GLTFImporter::GLTFImporter(const std::filesystem::path& file)
	{

	}

	MeshImporter::SceneMetadata GLTFImporter::GetSceneMetadata()
	{
		return MeshImporter::SceneMetadata();
	}

	std::vector<MeshImporter::TextureData> GLTFImporter::GetTextures()
	{
		return std::vector<MeshImporter::TextureData>();
	}

	std::vector<MeshImporter::MeshMetadata> GLTFImporter::GetMeshes()
	{
		return std::vector<MeshImporter::MeshMetadata>();
	}

	std::vector<MeshImporter::AnimationData> GLTFImporter::GetAnimations()
	{
		return std::vector<MeshImporter::AnimationData>();
	}

	MeshImporter::MeshData GLTFImporter::GetMeshData(const MeshMetadata& mesh, const std::function<void(uint32_t, uint32_t)>& progress)
	{
		return MeshData();
	}

	glm::mat4 GLTFImporter::GetMeshTransform(uint32_t meshIndex)
	{
		return glm::mat4();
	}
}
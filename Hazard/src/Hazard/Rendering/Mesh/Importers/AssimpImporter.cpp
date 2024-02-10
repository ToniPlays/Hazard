#include "AssimpImporter.h"

#include <assimp/postprocess.h>
#include "Callback.h"

namespace Hazard
{
	AssimpImporter::AssimpImporter(const std::filesystem::path& file) : m_SourcePath(file)
	{
		m_Importer.ReadFile(m_SourcePath.string().c_str(), aiPostProcessSteps::aiProcess_Triangulate);
	}

	MeshImporter::SceneMetadata AssimpImporter::GetSceneMetadata()
	{
		const aiScene* scene = m_Importer.GetScene();

		MeshImporter::SceneMetadata metadata = {};
		metadata.AnimationCount = scene->mNumAnimations;
		metadata.CameraCount = scene->mNumCameras;
		metadata.LightCount = scene->mNumLights;
		metadata.MeshCount = scene->mNumMeshes;
		metadata.MaterialCount = scene->mNumMaterials;
		metadata.TextureCount = scene->mNumTextures;

		return metadata;
	}

	std::vector<MeshImporter::TextureData> AssimpImporter::GetTextures()
	{
		const aiScene* scene = m_Importer.GetScene();
		std::vector<MeshImporter::TextureData> textures;
		textures.reserve(scene->mNumTextures);

		for (uint32_t i = 0; i < scene->mNumTextures; i++)
		{
			const aiTexture* texture = scene->mTextures[i];
			auto& data = textures.emplace_back();
			data.Filename = texture->mFilename.C_Str();
			data.TextureData = Buffer(texture->pcData, texture->mWidth);
		}
		return textures;
	}

	std::vector<MeshImporter::MeshMetadata> AssimpImporter::GetMeshes()
	{
		const aiScene* scene = m_Importer.GetScene();
		std::vector<MeshImporter::MeshMetadata> meshes;
		meshes.reserve(scene->mNumMeshes);

		for (uint32_t i = 0; i < scene->mNumMeshes; i++)
		{
			const aiMesh* mesh = scene->mMeshes[i];
			auto& data = meshes.emplace_back();

			data.Name = mesh->mName.C_Str();
			data.MaterialIndex = mesh->mMaterialIndex;
			data.VertexCount = mesh->mNumVertices;
			data.IndexCount = mesh->mNumFaces * 3;
			data.BoneCount = mesh->mNumBones;
			data.AnimatedMeshCount = mesh->mNumAnimMeshes;
			data.BoundingBox.MinX = mesh->mAABB.mMin.x;
			data.BoundingBox.MinY = mesh->mAABB.mMin.y;
			data.BoundingBox.MinZ = mesh->mAABB.mMin.z;
			data.BoundingBox.MaxX = mesh->mAABB.mMax.x;
			data.BoundingBox.MaxY = mesh->mAABB.mMax.y;
			data.BoundingBox.MaxZ = mesh->mAABB.mMax.z;
		}

		return meshes;
	}

	MeshImporter::MeshData AssimpImporter::GetMeshData(uint32_t meshIndex, const std::function<void(uint32_t, uint32_t)>& progress)
	{
		const aiScene* scene = m_Importer.GetScene();
		const aiMesh* mesh = scene->mMeshes[meshIndex];

		MeshData data = {};
		data.Name = mesh->mName.C_Str();
		data.Vertices.resize(mesh->mNumVertices);
		data.Indices.resize(mesh->mNumFaces * 3);
		//data.Transform = GetMeshTransform(meshIndex);	//TODO: Fix

		Callback<void(Vertex3D&, uint64_t)> loadCallback;
		loadCallback.Add([mesh](Vertex3D& vertex, uint64_t index) mutable {
			aiVector3D pos = mesh->mVertices[index];
			vertex.Position.x = pos.x;
			vertex.Position.y = pos.y;
			vertex.Position.z = pos.z;
		});

		for (uint32_t channel = 0; channel < AI_MAX_NUMBER_OF_COLOR_SETS; channel++)
		{
			if (mesh->HasVertexColors(channel))
			{
				loadCallback.Add([mesh, channel](Vertex3D& vertex, uint64_t index) mutable {
					aiColor4D color = mesh->mColors[channel][index];
					vertex.Color.r = color.r;
					vertex.Color.g = color.g;
					vertex.Color.b = color.b;
					vertex.Color.a = color.a;
				});
				break;
			}
		}

		if (mesh->HasNormals())
		{
			loadCallback.Add([mesh](Vertex3D& vertex, uint64_t index) mutable {
				aiVector3D normal = mesh->mNormals[index];
				vertex.Normals.x = normal.x;
				vertex.Normals.y = normal.y;
				vertex.Normals.z = normal.z;
			});
		}

		if (mesh->HasTangentsAndBitangents())
		{
			loadCallback.Add([mesh](Vertex3D& vertex, uint64_t index) mutable {
				aiVector3D tangent = mesh->mTangents[index];
				vertex.Tangent.x = tangent.x;
				vertex.Tangent.y = tangent.y;
				vertex.Tangent.z = tangent.z;

				aiVector3D bitangent = mesh->mBitangents[index];
				vertex.Binormal.x = bitangent.x;
				vertex.Binormal.y = bitangent.y;
				vertex.Binormal.z = bitangent.z;
			});
		}
		for (uint32_t channel = 0; channel < AI_MAX_NUMBER_OF_COLOR_SETS; channel++)
		{
			if (mesh->HasTextureCoords(channel))
			{
				loadCallback.Add([mesh, channel](Vertex3D& vertex, uint64_t index) mutable {
					aiVector3D coords = mesh->mTextureCoords[channel][index];
					vertex.TexCoords.x = coords.x;
					vertex.TexCoords.y = coords.y;
					vertex.TexCoords.z = coords.z;
				});
			}
		}

		for (uint32_t v = 0; v < mesh->mNumVertices; v++)
		{
			Vertex3D& vertex = data.Vertices[v];
			loadCallback.Invoke<Vertex3D&, uint64_t>(vertex, v);

			if (progress)
				progress(v, mesh->mNumVertices);
		}
		return data;
	}

	glm::mat4 AssimpImporter::GetMeshTransform(uint32_t meshIndex)
	{
		const aiScene* scene = m_Importer.GetScene();
		const aiNode* root = FindMeshNode(scene->mRootNode, meshIndex);

		if (!root) return glm::mat4(1.0f);

		while (root->mParent)
		{
			aiMatrix4x4 transform = root->mTransformation;
			root = root->mParent;
		}

		return glm::mat4(1.0f);
	}

	const aiNode* AssimpImporter::FindMeshNode(aiNode* root, uint32_t meshIndex)
	{
		const aiScene* scene = m_Importer.GetScene();
		const aiMesh* mesh = scene->mMeshes[meshIndex];
		const aiNode* node = root->FindNode(mesh->mName);

		if (node) return node;

		for (uint32_t i = 0; i < root->mNumChildren; i++)
		{
			const aiNode* child = FindMeshNode(root->mChildren[i], meshIndex);
			if (child)
				return child;
		}

		return nullptr;
	}

	std::vector<MeshImporter::AnimationData> AssimpImporter::GetAnimations()
	{
		std::vector<MeshImporter::AnimationData> animations;
		const aiScene* scene = m_Importer.GetScene();
		animations.reserve(scene->mNumAnimations);

		for (uint32_t i = 0; i < scene->mNumAnimations; i++)
		{
			const aiAnimation* animation = scene->mAnimations[i];
			auto& anim = animations.emplace_back();
			anim.Name = animation->mName.C_Str();
			anim.Duration = animation->mDuration;
		}

		return animations;
	}
}
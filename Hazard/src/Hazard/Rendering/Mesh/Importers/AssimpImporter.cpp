#include "AssimpImporter.h"

#include <assimp/postprocess.h>
#include "Callback.h"
#include "spdlog/fmt/fmt.h"

namespace Hazard
{
	AssimpImporter::AssimpImporter(const std::filesystem::path& file) : m_SourcePath(file)
	{

	}

	MeshImporter::SceneMetadata AssimpImporter::GetSceneMetadata()
	{
		const aiScene* scene = GetScene();

		MeshImporter::SceneMetadata metadata = {
			.AnimationCount = scene->mNumAnimations,
			.CameraCount = scene->mNumCameras,
			.LightCount = scene->mNumLights,
			.MeshCount = scene->mRootNode->mNumChildren,
			.MaterialCount = scene->mNumMaterials,
			.TextureCount = scene->mNumTextures,
		};

		PrintHierarchy(scene->mRootNode, 0);

		return metadata;
	}

	std::vector<MeshImporter::TextureData> AssimpImporter::GetTextures()
	{
		const aiScene* scene = GetScene();
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
		const aiScene* scene = GetScene();
		std::vector<MeshImporter::MeshMetadata> meshes;
		meshes.reserve(scene->mRootNode->mNumChildren);

		uint64_t vertexOffset = 0;
		uint64_t indexOffset = 0;

		for (uint32_t i = 0; i < scene->mRootNode->mNumChildren; i++)
		{
			const aiNode* node = scene->mRootNode->mChildren[i];
			if (node->mNumMeshes == 0) continue;

			uint64_t vertexCount = 0;
			uint64_t indexCount = 0;
			uint64_t materialIndex = 0;

			for (uint32_t m = 0; m < node->mNumMeshes; m++)
			{
				const aiMesh* mesh = scene->mMeshes[node->mMeshes[m]];
				vertexCount += mesh->mNumVertices;
				indexCount += mesh->mNumFaces * 3;
				materialIndex = mesh->mMaterialIndex;
			}

			auto& data = meshes.emplace_back();

			data.Name = node->mName.C_Str();
			data.NodeIndex = i;
			data.MaterialIndex = materialIndex;
			data.VertexCount = vertexCount;
			data.IndexCount = indexCount;
			data.VertexOffset = vertexOffset;
			data.IndexOffset = indexOffset;
			data.BoneCount = 0;
			data.AnimatedMeshCount = 0;

			vertexOffset += data.VertexCount;
			indexOffset += data.IndexCount;
		}

		return meshes;
	}

	MeshImporter::MeshData AssimpImporter::GetMeshData(const MeshMetadata& mesh, const std::function<void(uint32_t, uint32_t)>& progress)
	{
		const aiScene* scene = GetScene();
		const aiNode* node = scene->mRootNode->mChildren[mesh.NodeIndex];

		MeshData data = {
			.Name = mesh.Name,
		};

		auto transform = GetMeshAiTransform(node);

		for (uint32_t m = 0; m < node->mNumMeshes; m++)
		{
			const aiMesh* aiMesh = scene->mMeshes[node->mMeshes[m]];

			Callback<void(Vertex3D&, uint64_t)> loadCallback;
			loadCallback.Add([aiMesh, transform](Vertex3D& vertex, uint64_t index) mutable {
				aiVector3D pos = transform * aiMesh->mVertices[index];
				vertex.Position.x = pos.x;
				vertex.Position.y = pos.y;
				vertex.Position.z = pos.z;
			});

			for (uint32_t channel = 0; channel < AI_MAX_NUMBER_OF_COLOR_SETS; channel++)
			{
				if (aiMesh->HasVertexColors(channel))
				{
					loadCallback.Add([aiMesh, channel](Vertex3D& vertex, uint64_t index) mutable {
						aiColor4D color = aiMesh->mColors[channel][index];
						vertex.Color.r = color.r;
						vertex.Color.g = color.g;
						vertex.Color.b = color.b;
						vertex.Color.a = color.a;
					});
					break;
				}
			}

			if (aiMesh->HasNormals())
			{
				loadCallback.Add([aiMesh](Vertex3D& vertex, uint64_t index) mutable {
					aiVector3D normal = aiMesh->mNormals[index];
					vertex.Normals.x = normal.x;
					vertex.Normals.y = normal.y;
					vertex.Normals.z = normal.z;
				});
			}

			if (aiMesh->HasTangentsAndBitangents())
			{
				loadCallback.Add([aiMesh](Vertex3D& vertex, uint64_t index) mutable {
					aiVector3D tangent = aiMesh->mTangents[index];
					vertex.Tangent.x = tangent.x;
					vertex.Tangent.y = tangent.y;
					vertex.Tangent.z = tangent.z;

					aiVector3D bitangent = aiMesh->mBitangents[index];
					vertex.Binormal.x = bitangent.x;
					vertex.Binormal.y = bitangent.y;
					vertex.Binormal.z = bitangent.z;
				});
			}
			for (uint32_t channel = 0; channel < AI_MAX_NUMBER_OF_COLOR_SETS; channel++)
			{
				if (aiMesh->HasTextureCoords(channel))
				{
					loadCallback.Add([aiMesh, channel](Vertex3D& vertex, uint64_t index) mutable {
						aiVector3D coords = aiMesh->mTextureCoords[channel][index];
						vertex.TexCoords.x = coords.x;
						vertex.TexCoords.y = coords.y;
						vertex.TexCoords.z = coords.z;
					});
				}
			}

			for (uint32_t v = 0; v < aiMesh->mNumVertices; v++)
			{
				Vertex3D& vertex = data.Vertices.emplace_back();
				loadCallback.Invoke<Vertex3D&, uint64_t>(vertex, v);

				if (progress)
					progress(v, aiMesh->mNumVertices);
			}

			for (uint64_t f = 0; f < aiMesh->mNumFaces; f++)
			{
				aiFace face = aiMesh->mFaces[f];
				for (uint32_t index = 0; index < face.mNumIndices; index++)
					data.Indices.push_back(face.mIndices[index]);
			}
		}
		return data;
	}

	glm::mat4 AssimpImporter::GetMeshTransform(uint32_t nodeIndex)
	{
		return glm::mat4(1.0f);
	}

	aiMatrix4x4 AssimpImporter::GetMeshAiTransform(const aiNode* node)
	{
		const aiScene* scene = GetScene();
		if (!node) return aiMatrix4x4();

		aiMatrix4x4 transform = node->mTransformation;
		while (node->mParent)
		{
			transform = node->mParent->mTransformation * transform;
			node = node->mParent;
		}
		return transform;
	}

	const aiNode* AssimpImporter::FindMeshNode(aiNode* root, uint32_t meshIndex)
	{
		const aiScene* scene = GetScene();
		const aiMesh* mesh = scene->mMeshes[meshIndex];
		
		for (uint32_t i = 0; i < root->mNumChildren; i++)
		{
			const aiNode* child = root->mChildren[i];
			for (uint32_t m = 0; m < child->mNumMeshes; m++)
			{
				const aiMesh* aiMesh = scene->mMeshes[child->mMeshes[m]];
				if (aiMesh == mesh) 
					return child;
			}
		}

		HZR_ASSERT(false, fmt::format("Missing node: {}", mesh->mName.C_Str()));
		return nullptr;
	}

	const aiScene* AssimpImporter::GetScene()
	{
		const aiScene* scene = m_Importer.GetScene();
		if (scene) return scene;

		uint32_t flags = aiProcess_Triangulate;
		flags |= aiProcess_JoinIdenticalVertices;
		flags |= aiProcess_GenSmoothNormals;
		flags |= aiProcess_SplitLargeMeshes;

		m_Importer.SetProgressHandler(new AssimpProgressHandler([this](float progress) {
			m_LoadCallback.Invoke(progress);
		}));

		scene = m_Importer.ReadFile(m_SourcePath.string().c_str(), flags);

		m_LoadCallback.Clear();
		return scene;
	}

	void AssimpImporter::PrintHierarchy(const aiNode* node, uint32_t level)
	{
		const aiScene* scene = GetScene();
		std::cout << fmt::format("Node: {}", node->mName.C_Str()) << std::endl;

		for (uint32_t i = 0; i < node->mNumChildren; i++)
		{
			aiNode* child = node->mChildren[i];
			std::cout << fmt::format("  Node: {}", child->mName.C_Str()) << std::endl;
			std::cout << fmt::format("    Parent: {}", child->mParent->mName.C_Str()) << std::endl;
			std::cout << fmt::format("    Meshes: {}", child->mNumMeshes) << std::endl;
			for (uint32_t m = 0; m < child->mNumMeshes; m++)
			{
				int meshIndex = child->mMeshes[m];
				const aiMesh* mesh = scene->mMeshes[meshIndex];
				std::cout << fmt::format("      {}, material: {}", mesh->mName.C_Str(), mesh->mMaterialIndex) << std::endl;
			}
		}
	}

	std::vector<MeshImporter::AnimationData> AssimpImporter::GetAnimations()
	{
		std::vector<MeshImporter::AnimationData> animations;
		const aiScene* scene = GetScene();
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
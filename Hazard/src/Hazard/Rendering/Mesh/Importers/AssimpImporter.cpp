#include "AssimpImporter.h"

#include <assimp/postprocess.h>
#include "Callback.h"
#include "spdlog/fmt/fmt.h"
#include <Hooks.h>

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
			.MeshCount = scene->mNumMeshes,
			.MaterialCount = scene->mNumMaterials,
			.TextureCount = scene->mNumTextures,
		};

		//PrintHierarchy(scene->mRootNode, 0);

		return metadata;
	}

	std::vector<MeshImporter::TextureMetadata> AssimpImporter::GetTextures()
	{
		std::vector<MeshImporter::TextureMetadata> textures;
		const aiScene* scene = GetScene();
		textures.reserve(scene->mNumTextures);

		for (uint32_t i = 0; i < scene->mNumTextures; i++)
		{
			const aiTexture* texture = scene->mTextures[i];
			auto& data = textures.emplace_back();
			data.Name = texture->mFilename.C_Str();
			data.TextureIndex = i;
		}
		return textures;
	}

	std::vector<MeshImporter::MaterialMetadata> AssimpImporter::GetMaterials()
	{
		const aiScene* scene = GetScene();
		std::vector<MeshImporter::MaterialMetadata> materials;

		for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; materialIndex++)
		{
			aiMaterial* mat = scene->mMaterials[materialIndex];

			auto& material = materials.emplace_back();
			material.Name = mat->GetName().C_Str();
			material.MaterialIndex = materialIndex;
			material.PropertyCount = mat->mNumProperties;
			material.Textures = GetMaterialTextures(mat);
		}

		return materials;
	}

	std::vector<MeshImporter::MeshMetadata> AssimpImporter::GetMeshes()
	{
		const aiScene* scene = GetScene();
		std::vector<MeshImporter::MeshMetadata> meshes;
		meshes.reserve(scene->mRootNode->mNumChildren + 1);

		uint64_t vertexOffset = 0;
		uint64_t indexOffset = 0;

		{
			auto data = GetMeshDataFromNode(scene->mRootNode);
			data.NodeIndex = 0;
			data.VertexOffset = vertexOffset;
			data.IndexOffset = indexOffset;

			if (data.IndexCount > 0)
			{
				meshes.push_back(data);

				vertexOffset = data.VertexCount;
				indexOffset = data.IndexCount;
			}
		}

		for (uint32_t i = 0; i < scene->mRootNode->mNumChildren; i++)
		{
			aiNode* node = scene->mRootNode->mChildren[i];
			auto data = GetMeshDataFromNode(node);

			data.NodeIndex = i + 1;
			data.VertexOffset = vertexOffset;
			data.IndexCount = indexOffset;

			if (data.VertexCount == 0) continue;

			meshes.push_back(data);
			vertexOffset = data.VertexCount;
			indexOffset = data.IndexCount;
		}

		return meshes;
	}

	MeshImporter::MeshData AssimpImporter::GetMeshData(const MeshMetadata& mesh, const std::function<void(uint32_t, uint32_t)>& progress)
	{
		const aiScene* scene = GetScene();
		const aiNode* node = mesh.NodeIndex == 0 ? scene->mRootNode : scene->mRootNode->mChildren[mesh.NodeIndex - 1];

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

	MeshImporter::MaterialData AssimpImporter::GetMaterial(uint32_t materialIndex)
	{
		const aiScene* scene = GetScene();
		aiMaterial* mat = scene->mMaterials[materialIndex];

		std::vector<MeshImporter::MaterialProperty> properties;

		Hooks<std::string, void(aiMaterialProperty*)> hooks;
		hooks.AddHook("$clr.ambient", [&properties](aiMaterialProperty* property) {
			auto& prop = properties.emplace_back();
			prop.Name = property->mKey.C_Str();
			prop.Usage = TextureType::Albedo;
			prop.Type = AiPropertyToShaderType(*property);
			prop.Data = Buffer::Copy(property->mData, property->mDataLength);
		});

		hooks.AddHook("$clr.diffuse", [&properties](aiMaterialProperty* property) {
			auto& prop = properties.emplace_back();
			prop.Name = property->mKey.C_Str();
			prop.Usage = TextureType::Diffuse;
			prop.Type = AiPropertyToShaderType(*property);
			prop.Data = Buffer::Copy(property->mData, property->mDataLength);
		});

		hooks.AddHook("$clr.specular", [&properties](aiMaterialProperty* property) {
			auto& prop = properties.emplace_back();
			prop.Name = property->mKey.C_Str();
			prop.Usage = TextureType::Specular;
			prop.Type = AiPropertyToShaderType(*property);
			prop.Data = Buffer::Copy(property->mData, property->mDataLength);
		});

		hooks.AddHook("$clr.emissive", [&properties](aiMaterialProperty* property) {
			auto& prop = properties.emplace_back();
			prop.Name = property->mKey.C_Str();
			prop.Usage = TextureType::Emission;
			prop.Type = AiPropertyToShaderType(*property);
			prop.Data = Buffer::Copy(property->mData, property->mDataLength);
		});

		hooks.AddHook("$mat.gltf.pbrMetallicRoughness.metallicFactor", [&properties](aiMaterialProperty* property) {
			auto& prop = properties.emplace_back();
			prop.Name = property->mKey.C_Str();
			prop.Usage = TextureType::Metalness;
			prop.Type = AiPropertyToShaderType(*property);
			prop.Data = Buffer::Copy(property->mData, property->mDataLength);
		});

		hooks.AddHook("$mat.gltf.pbrMetallicRoughness.roughnessFactor", [&properties](aiMaterialProperty* property) {
			auto& prop = properties.emplace_back();
			prop.Name = property->mKey.C_Str();
			prop.Usage = TextureType::Metalness;
			prop.Type = AiPropertyToShaderType(*property);
			prop.Data = Buffer::Copy(property->mData, property->mDataLength);
		});

		
		for (uint32_t i = 0; i < mat->mNumProperties; i++)
		{
			aiMaterialProperty* property = mat->mProperties[i];
			std::string key = property->mKey.C_Str();
			//std::cout << key << std::endl;
			hooks.Invoke(key, property);
		}

		MeshImporter::MaterialData data = {
			.Name = mat->GetName().C_Str(),
			.Properties = properties
		};
		
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

	MeshImporter::MeshMetadata AssimpImporter::GetMeshDataFromNode(aiNode* node)
	{
		if (node->mNumMeshes == 0)
			return MeshImporter::MeshMetadata();

		const aiScene* scene = GetScene();

		uint64_t vertexCount = 0;
		uint64_t indexCount = 0;
		uint32_t materialIndex = 0;

		for (uint32_t m = 0; m < node->mNumMeshes; m++)
		{
			const aiMesh* mesh = scene->mMeshes[node->mMeshes[m]];
			vertexCount += mesh->mNumVertices;
			indexCount += mesh->mNumFaces * 3;
			materialIndex = mesh->mMaterialIndex;
		}

		MeshImporter::MeshMetadata data = {
			.Name = node->mName.C_Str(),
			.MaterialIndex = materialIndex,
			.VertexCount = vertexCount,
			.IndexCount = indexCount,
			.BoneCount = 0,
			.AnimatedMeshCount = 0,
		};

		return data;
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

	std::unordered_map<MeshImporter::TextureType, uint32_t> AssimpImporter::GetMaterialTextures(aiMaterial* material)
	{
		std::unordered_map<TextureType, uint32_t> result;

		Hooks<aiTextureType, void(uint32_t)> hooks;

		hooks.AddHook(aiTextureType_BASE_COLOR, [&result](uint32_t count) mutable { result[TextureType::Albedo] = count; });
		hooks.AddHook(aiTextureType_DIFFUSE, [&result](uint32_t count) mutable { result[TextureType::Diffuse] = count; });
		hooks.AddHook(aiTextureType_SPECULAR, [&result](uint32_t count) mutable { result[TextureType::Specular] = count; });
		hooks.AddHook(aiTextureType_EMISSION_COLOR, [&result](uint32_t count) mutable { result[TextureType::Emission] = count; });
		hooks.AddHook(aiTextureType_METALNESS, [&result](uint32_t count) mutable { result[TextureType::Metalness] = count; });
		hooks.AddHook(aiTextureType_NORMALS, [&result](uint32_t count) mutable { result[TextureType::Normal] = count; });
		hooks.AddHook(aiTextureType_HEIGHT, [&result](uint32_t count) mutable { result[TextureType::Height] = count; });
		hooks.AddHook(aiTextureType_SHININESS, [&result](uint32_t count) mutable { result[TextureType::Shininess] = count; });
		hooks.AddHook(aiTextureType_DISPLACEMENT, [&result](uint32_t count) mutable { result[TextureType::Displacement] = count; });
		hooks.AddHook(aiTextureType_LIGHTMAP, [&result](uint32_t count) mutable { result[TextureType::Lightmap] = count; });
		hooks.AddHook(aiTextureType_REFLECTION, [&result](uint32_t count) mutable { result[TextureType::Reflection] = count; });

		for (uint32_t i = aiTextureType_DIFFUSE; i < aiTextureType_UNKNOWN - 1; i++)
		{
			uint32_t count = material->GetTextureCount((aiTextureType)i);
			if (count == 0) continue;

			hooks.Invoke((aiTextureType)i, count);
		}

		return result;
	}

	HazardRenderer::ShaderDataType AssimpImporter::AiPropertyToShaderType(const aiMaterialProperty& material)
	{
		using namespace HazardRenderer;

		switch (material.mType)
		{
			case aiPTI_Float: 
			{
				switch (material.mDataLength)
				{
					case sizeof(float) * 1: return ShaderDataType::Float;
					case sizeof(float) * 2: return ShaderDataType::Float2;
					case sizeof(float) * 3: return ShaderDataType::Float3;
					case sizeof(float) * 4: return ShaderDataType::Float4;
				}
			}
			case aiPTI_Integer:
			{
				switch (material.mDataLength)
				{
					case sizeof(int) * 1: return ShaderDataType::Int;
					case sizeof(int) * 2: return ShaderDataType::Int2;
					case sizeof(int) * 3: return ShaderDataType::Int3;
					case sizeof(int) * 4: return ShaderDataType::Int4;
				}
			}
		}
		return ShaderDataType::None;
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
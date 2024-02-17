#include "AssimpImporter.h"

#include "Hazard/RenderContext/TextureFactory.h"

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
			material.TextureCount = 0;
		}

		return materials;
	}

	std::vector<MeshImporter::MeshMetadata> AssimpImporter::GetMeshes()
	{
		const aiScene* scene = GetScene();
		std::vector<MeshImporter::MeshMetadata> meshes;
		meshes.reserve(scene->mNumMeshes);

		for (uint32_t m = 0; m < scene->mNumMeshes; m++)
		{
			const aiMesh* mesh = scene->mMeshes[m];

			MeshMetadata data = {
				.Name = mesh->mName.C_Str(),
				.MeshIndex = m,
				.MaterialIndex = mesh->mMaterialIndex,
				.VertexCount = mesh->mNumVertices,
				.IndexCount = mesh->mNumFaces * 3,
				.BoneCount = mesh->mNumBones,
				.AnimatedMeshCount = mesh->mNumAnimMeshes,
			};

			meshes.push_back(data);
		}

		return meshes;
	}

	MeshImporter::MeshData AssimpImporter::GetMeshData(const MeshMetadata& mesh, const std::function<void(uint32_t, uint32_t)>& progress)
	{
		const aiScene* scene = GetScene();
		aiMesh* aiMesh = scene->mMeshes[mesh.MeshIndex];

		MeshData data = {
			.Name = mesh.Name,
			.Transform = GetMeshTransform(mesh.MeshIndex)
		};

		Callback<void(Vertex3D&, uint64_t)> loadCallback;
		loadCallback.Add([aiMesh](Vertex3D& vertex, uint64_t index) mutable {
			aiVector3D pos = aiMesh->mVertices[index];
			vertex.Position.x = pos.x;
			vertex.Position.y = pos.y;
			vertex.Position.z = pos.z;
		});

		for (uint32_t channel = 0; channel < aiMesh->GetNumColorChannels(); channel++)
		{
			if (aiMesh->HasVertexColors(channel))
			{
				loadCallback.Add([aiMesh, channel](Vertex3D& vertex, uint64_t index) mutable {
					aiColor4D color = aiMesh->mColors[channel][index];
					if (isnan(color.r) || isnan(color.g) || isnan(color.b) || isnan(color.a)) return;

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

		data.MaterialIndex = aiMesh->mMaterialIndex;
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
		const aiScene* scene = GetScene();
		auto t = GetMeshAiTransform(scene->mMeshes[nodeIndex]);

		return glm::mat4{ t[0][0], t[1][0], t[2][0], t[3][0],
						  t[0][1], t[1][1], t[2][1], t[3][1],
						  t[0][2], t[1][2], t[2][2], t[3][2],
						  t[0][3], t[1][3], t[2][3], t[3][3]
		};
	}

	aiMatrix4x4 AssimpImporter::GetMeshAiTransform(aiMesh* mesh)
	{
		const aiScene* scene = GetScene();
		const aiNode* root = FindMeshNode(scene->mRootNode, mesh);
		aiMatrix4x4 transform = root->mTransformation;

		while (root->mParent)
		{
			transform = root->mParent->mTransformation * transform;
			root = root->mParent;
		}
		return transform;
	}

	const aiNode* AssimpImporter::FindMeshNode(aiNode* root, aiMesh* mesh)
	{
		const aiScene* scene = GetScene();

		for (uint32_t i = 0; i < root->mNumChildren; i++)
		{
			aiNode* child = root->mChildren[i];

			for (uint32_t m = 0; m < child->mNumMeshes; m++)
			{
				aiMesh* aiMesh = scene->mMeshes[child->mMeshes[m]];
				if (aiMesh == mesh)
					return child;
			}
		}

		for (uint32_t m = 0; m < scene->mNumMeshes; m++)
		{
			aiMesh* aiMesh = scene->mMeshes[m];

			if (aiMesh == mesh)
				return scene->mRootNode;
		}

		HZR_ASSERT(false, "Not found");
		return scene->mRootNode;
	}

	const aiScene* AssimpImporter::GetScene()
	{
		const aiScene* scene = m_Importer.GetScene();
		if (scene) return scene;

		uint32_t flags = aiProcess_Triangulate;
		flags |= aiProcess_JoinIdenticalVertices;
		flags |= aiProcess_GenSmoothNormals;
		flags |= aiProcess_SplitLargeMeshes;
		flags |= aiProcess_ValidateDataStructure;
		flags |= aiProcess_OptimizeMeshes;
		flags |= aiProcess_RemoveRedundantMaterials;

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
		std::cout << fmt::format("Meshes: {}", node->mNumMeshes) << std::endl;

		for (uint32_t m = 0; m < node->mNumMeshes; m++)
		{
			int meshIndex = node->mMeshes[m];
			const aiMesh* mesh = scene->mMeshes[meshIndex];
			std::cout << fmt::format("      \"{}\", material: {} ({})", mesh->mName.C_Str(), mesh->mMaterialIndex, (void*)mesh) << std::endl;
		}

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
				std::cout << fmt::format("      \"{}\", material: {} ({})", mesh->mName.C_Str(), mesh->mMaterialIndex, (void*)mesh) << std::endl;
			}
		}
	}

	std::unordered_map<MeshImporter::TextureType, uint32_t> AssimpImporter::GetMaterialTextures(aiMaterial* material)
	{
		const aiScene* scene = GetScene();

		std::unordered_map<TextureType, uint32_t> result;

		Hooks<aiTextureType, void(uint32_t)> hooks;

		hooks.AddHook(aiTextureType_BASE_COLOR, [&result](uint32_t index) mutable { result[TextureType::Albedo] = index; });
		hooks.AddHook(aiTextureType_DIFFUSE, [&result](uint32_t index) mutable { result[TextureType::Diffuse] = index; });
		hooks.AddHook(aiTextureType_SPECULAR, [&result](uint32_t index) mutable { result[TextureType::Specular] = index; });
		hooks.AddHook(aiTextureType_EMISSION_COLOR, [&result](uint32_t index) mutable { result[TextureType::Emission] = index; });
		hooks.AddHook(aiTextureType_METALNESS, [&result](uint32_t index) mutable { result[TextureType::Metalness] = index; });
		hooks.AddHook(aiTextureType_NORMALS, [&result](uint32_t index) mutable { result[TextureType::Normal] = index; });
		hooks.AddHook(aiTextureType_HEIGHT, [&result](uint32_t index) mutable { result[TextureType::Height] = index; });
		hooks.AddHook(aiTextureType_SHININESS, [&result](uint32_t index) mutable { result[TextureType::Shininess] = index; });
		hooks.AddHook(aiTextureType_DISPLACEMENT, [&result](uint32_t index) mutable { result[TextureType::Displacement] = index; });
		hooks.AddHook(aiTextureType_LIGHTMAP, [&result](uint32_t index) mutable { result[TextureType::Lightmap] = index; });
		hooks.AddHook(aiTextureType_REFLECTION, [&result](uint32_t index) mutable { result[TextureType::Reflection] = index; });

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
	MeshImporter::TextureData AssimpImporter::GetTextureData(uint32_t textureIndex)
	{
		const aiScene* scene = GetScene();
		aiTexture* texture = scene->mTextures[textureIndex];

		if (texture->mHeight == 0)
		{
			TextureHeader header = TextureFactory::LoadTextureFromMemory({ texture->pcData, texture->mWidth });

			MeshImporter::TextureData data = {
				.Name = texture->mFilename.C_Str(),
				.Width = header.Extent.Width,
				.Height = header.Extent.Height,
				.ImageData = header.ImageData,
			};

			return data;
		}
		return MeshImporter::TextureData();
	}
}
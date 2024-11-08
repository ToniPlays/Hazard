
#include <hzrpch.h>

#include "MeshAssetLoader.h"
#include "Importers/AssimpImporter.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Assets/AssetPack.h"
#include "MaterialAssetLoader.h"
#include "Material.h"

#include "Hazard/Core/Application.h"
#include "Mesh.h"
#include <Filesystem/Directory.h>

namespace Hazard
{
	struct MeshDependencyData
	{
		MeshImporter::MeshData MeshData;
		AssetHandle Handle = INVALID_ASSET_HANDLE;
		uint64_t MaterialIndex = UINT64_MAX;
		std::string TextureName;
	};

	Ref<JobGraph> MeshAssetLoader::Load(AssetMetadata& metadata, const LoadAssetSettings& settings)
	{
		HZR_PROFILE_FUNCTION();

		//Ref<Job> loadJob = Job::Create("Load", CreateMeshFromSource, metadata.Handle);

		JobGraphInfo info = {
			.Name = "Mesh load",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "Source load", 1.0f, { }} },
		};

		return Ref<JobGraph>::Create(info);
	}

	Ref<JobGraph> MeshAssetLoader::Save(Ref<Asset> asset, const SaveAssetSettings& settings)
	{
		//Ref<Job> saveJob = Job::Create("Save", ReadMeshDataFromGPU, asset.As<Mesh>());
		//Ref<Job> compileMesh = Job::Create("Mesh compile", CompileMesh, asset.As<Mesh>());

		JobGraphInfo info = {
			.Name = "Mesh save",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "Process", 0.8f, { } },
						{ "Compile", 0.2f, { } }
			},
		};

		return Ref<JobGraph>::Create(info);
	}

	Ref<JobGraph> MeshAssetLoader::Create(const CreateAssetSettings& settings)
	{
		CreateSettings importSettings;
		if (settings.Settings != nullptr)
			importSettings = *(CreateSettings*)settings.Settings;

		Ref<AssimpImporter> importer = Ref<AssimpImporter>::Create(settings.SourcePath);

		//Ref<Job> preprocessJob = Job::Create(fmt::format("Mesh: {}", settings.SourcePath.string()), PreprocessDependencies, importer, importSettings);
		//Ref<Job> finalize = Job::Create(fmt::format("Finalize Mesh: {}", settings.SourcePath.string()), FinalizeMesh, importer);

		JobGraphInfo info = {
			.Name = "Mesh create",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "Preprocess", 0.1f, { } },
						{ "Dependency load", 0.8f, { } },
						{ "Finalize", 0.1f, { } }
			},
		};

		return Ref<JobGraph>::Create(info);
	}

	void MeshAssetLoader::PreprocessDependencies(JobInfo& info, Ref<MeshImporter> importer, const CreateSettings& settings)
	{
		Job& job = *info.Current.Raw();

		importer->AddImportProgressCallback([&job](float progress) {
			job.Progress(progress);
		});

		auto metadata = importer->GetSceneMetadata();
		if (metadata.MeshCount == 0)
			throw JobException("No meshes found in file");

		auto meshes = importer->GetMeshes();
		auto animations = importer->GetAnimations();

		std::vector<Ref<Job>> generateJobs;
		generateJobs.reserve(meshes.size());

		for (auto& mesh : meshes)
		{
			//Ref<Job> processMeshJob = Job::Create(fmt::format("Mesh: {}", mesh.Name), ProcessMeshNode, importer, mesh);
			//generateJobs.push_back(processMeshJob);
		}

		if (settings.Flags & MESH_CREATE_INCLUDE_MATERIALS)
		{
			auto materials = importer->GetMaterials();
			for (auto& material : materials)
			{
				//Ref<Job> processMeshJob = Job::Create(fmt::format("Material: {}", material.Name), ProcessMaterial, importer, material, settings.MaterialPath);
				//generateJobs.push_back(processMeshJob);
			}

			auto textures = importer->GetTextures();
			for (auto& texture : textures)
			{
				//Ref<Job> processMeshJob = Job::Create(fmt::format("Material: {}", texture.Name), ProcessTexture, importer, texture, settings.TexturePath);
				//generateJobs.push_back(processMeshJob);
			}
		}

		info.ContinueWith(generateJobs);
	}

	void MeshAssetLoader::ProcessMeshNode(JobInfo& info, Ref<MeshImporter> importer, const MeshImporter::MeshMetadata& mesh)
	{
		Job& jobRef = *info.Current;
		MeshImporter::MeshData data = importer->GetMeshData(mesh, [&jobRef](uint32_t current, uint32_t total) mutable {
			jobRef.Progress((float)current / (float)total);
			});

		MeshDependencyData result = {
			.MeshData = data,
		};

		info.Result(result);
	}

	void MeshAssetLoader::ProcessMaterial(JobInfo& info, Ref<MeshImporter> importer, const MeshImporter::MaterialMetadata& material, const std::filesystem::path& materialRoot)
	{
		MaterialAssetLoader::CreateSettings mat = {};

		CreateAssetSettings settings = {
			.Type = AssetType::Material,
			.SourcePath = materialRoot / (material.Name + ".hasset"),
			.Settings = &mat,
		};

		std::atomic_bool fence = true;

		auto props = importer->GetMaterial(material.MaterialIndex);

		AssetManager::CreateAssetAsync<Material>(settings).ContinueWith([settings, props, material, info](const auto& results) {
			Ref<Material> asset = results[0];
			SetMaterialProperties(asset, props);

			SaveAssetSettings saveSettings = {
				.TargetPath = settings.SourcePath,
				.Flags = ASSET_MANAGER_COMBINE_ASSET | ASSET_MANAGER_SAVE_AND_UPDATE,
			};

			MeshDependencyData result = {
				.Handle = asset->GetHandle(),
				.MaterialIndex = material.MaterialIndex
			};

			AssetManager::SaveAsset(asset, saveSettings).ContinueWith([info, asset, r = result](const auto& result) mutable {
				info.Result(result);
				});
			});
	}

	void MeshAssetLoader::ProcessTexture(JobInfo& info, Ref<MeshImporter> importer, const MeshImporter::TextureMetadata& texture, const std::filesystem::path& textureRoot)
	{
		CreateAssetSettings create = {
			.Type = AssetType::Image,
		};

		Ref<Texture2DAsset> asset = AssetManager::CreateAsset<Texture2DAsset>(create);

		auto textureData = importer->GetTextureData(texture.TextureIndex);
		asset->SetExtent({ textureData.Width, textureData.Height, 1 });
		asset->SetMaxMipLevels(1);
		asset->Invalidate(textureData.ImageData);

		SaveAssetSettings saveSettings = {
				.TargetPath = textureRoot / (textureData.Name + ".hasset"),
				.Flags = ASSET_MANAGER_COMBINE_ASSET | ASSET_MANAGER_SAVE_AND_UPDATE,
		};

		MeshDependencyData result = {
			.Handle = asset->GetHandle(),
			.TextureName = textureData.Name,
		};

		AssetManager::SaveAsset(asset, saveSettings);

		info.Result(result);
		textureData.ImageData.Release();
	}

	void MeshAssetLoader::FinalizeMesh(JobInfo& info, Ref<MeshImporter> importer)
	{
		std::vector<MeshDependencyData> results = info.Graph->GetResults<MeshDependencyData>();

		Ref<Mesh> mesh = Ref<Mesh>::Create();

		std::vector<MeshImporter::MeshData> meshData;
		std::unordered_map<uint64_t, AssetHandle> materialData;
		std::unordered_map<std::string, AssetHandle> textures;

		for (auto& result : results)
		{
			if (result.MeshData.Vertices.size() > 0)
				meshData.push_back(result.MeshData);

			if (result.MaterialIndex != UINT64_MAX)
				materialData[result.MaterialIndex] = result.Handle;

			if (result.Handle)
				textures[result.TextureName] = result.Handle;

		}

		mesh->GenerateMesh(meshData);

		//Set materials
		for (auto& submesh : meshData)
		{
			if (!materialData.contains(submesh.MaterialIndex)) continue;

			uint64_t nodeID = mesh->GetSubmeshNodeFromName(submesh.Name);
			AssetHandle handle = materialData[submesh.MaterialIndex];
			mesh->SetSubmeshMaterialHandle(nodeID, handle);
		}

		info.Result(mesh);
	}

	void MeshAssetLoader::ReadMeshDataFromGPU(JobInfo& info, Ref<Mesh> mesh)
	{
		using namespace HazardRenderer;

		BufferCreateInfo vertexReadbackSpec = {
			.Name = "Mesh vertex readback",
			.UsageFlags = BUFFER_USAGE_VERTEX_BUFFER_BIT | BUFFER_USAGE_DYNAMIC,
			.Size = mesh->CalculateTotalVertexCount() * sizeof(Vertex3D),
		};

		BufferCreateInfo indexReadbackSpec = {
			.Name = "Mesh index readback",
			.UsageFlags = BUFFER_USAGE_INDEX_BUFFER_BIT | BUFFER_USAGE_DYNAMIC,
			.Size = mesh->CalculateTotalIndexCount() * sizeof(uint32_t),
		};

		Ref<GPUBuffer> vertexReadback = GPUBuffer::Create(&vertexReadbackSpec);
		Ref<GPUBuffer> indexReadback = GPUBuffer::Create(&indexReadbackSpec);

		Ref<RenderCommandBuffer> cmdBuffer = RenderCommandBuffer::Create("Mesh readback", DeviceQueue::TransferBit, 1);

		cmdBuffer->Begin();

		for (auto& [node, submesh] : mesh->GetSubmeshData())
		{
			BufferCopyRegion vertexSrcRegion = {
				.Size = submesh.VertexCount * sizeof(Vertex3D),
				.Offset = 0,
			};

			BufferCopyRegion indexSrcRegion = {
				.Size = submesh.IndexCount * sizeof(uint32_t),
				.Offset = 0,
			};

			BufferCopyRegion vertexDstRegion = {
				.Size = submesh.VertexCount * sizeof(Vertex3D),
				.Offset = submesh.VertexOffset,
			};

			BufferCopyRegion indexDstRegion = {
				.Size = submesh.IndexCount * sizeof(uint32_t),
				.Offset = submesh.IndexOffset,
			};

			cmdBuffer->CopyBufferToBuffer(mesh->GetVertexBuffer(submesh.NodeID), vertexSrcRegion, vertexReadback, vertexDstRegion);
			cmdBuffer->CopyBufferToBuffer(mesh->GetIndexBuffer(submesh.NodeID), indexSrcRegion, indexReadback, indexDstRegion);
		}
		cmdBuffer->End();
		cmdBuffer->Submit();
		cmdBuffer->OnCompleted([info, vertexReadback, indexReadback]() mutable {
			struct Result
			{
				Buffer Vertex;
				Buffer Index;
			} result;

			BufferCopyRegion vertexRegion = {
				.Size = vertexReadback->GetSize(),
			};

			BufferCopyRegion indexRegion = {
				.Size = indexReadback->GetSize(),
			};

			result.Vertex = Buffer::Copy(vertexReadback->ReadData(vertexRegion));
			result.Index = Buffer::Copy(indexReadback->ReadData(indexRegion));

			info.Result(result);

		});
	}

	void MeshAssetLoader::CompileMesh(JobInfo& info, Ref<Mesh> mesh)
	{
		struct Result
		{
			Buffer Vertex;
			Buffer Index;
		} result = info.Graph->GetResults<Result>()[0];

		auto meshData = mesh->GetSubmeshData();

		uint64_t nodeNameLength = 0;
		for (auto& [node, submesh] : meshData)
			nodeNameLength += submesh.NodeName.length() + sizeof(uint64_t);

		Ref<CachedBuffer> buf = Ref<CachedBuffer>::Create();
		buf->Allocate(sizeof(MeshFileHeader) + sizeof(SubmeshHeader) * meshData.size() + nodeNameLength + result.Vertex.Size + result.Index.Size);

		MeshFileHeader meshHeader = {
			.SubmeshCount = meshData.size(),
			.VertexCount = result.Vertex.Size / sizeof(Vertex3D),
			.IndexCount = result.Index.Size / sizeof(uint32_t),
		};

		buf->Write(meshHeader);

		uint32_t progress = 0;
		for (auto& [node, submesh] : meshData)
		{
			SubmeshHeader header = {
				.NodeID = submesh.NodeID,
				.Transform = submesh.Transform,
				.MaterialHandle = submesh.DefaultMaterialHandle,
				.VertexCount = submesh.VertexCount,
				.IndexCount = submesh.IndexCount,
				.VertexOffset = submesh.VertexOffset,
				.IndexOffset = submesh.IndexOffset,
			};

			buf->Write(header);
			buf->Write(submesh.NodeName);

			progress++;
			info.Current->Progress((float)progress / (float)meshData.size());
		}

		buf->Write<Buffer>(result.Vertex);
		buf->Write<Buffer>(result.Index);
		info.Result(buf);

		result.Vertex.Release();
		result.Index.Release();
	}

	void MeshAssetLoader::CreateMeshFromSource(JobInfo& info, AssetHandle handle)
	{
		AssetMetadata& metadata = AssetManager::GetMetadata(handle);

		if (!File::Exists(metadata.FilePath))
			throw JobException("File does not exist");

		Ref<CachedBuffer> buffer = File::ReadBinaryFile(metadata.FilePath);
		AssetPack pack = {};
		pack.FromBuffer(buffer);

		MeshFileHeader header = pack.AssetData->Read<MeshFileHeader>();

		std::vector<SubmeshData> submeshes;

		for (uint32_t i = 0; i < header.SubmeshCount; i++)
		{
			SubmeshHeader subHeader = pack.AssetData->Read<SubmeshHeader>();
			std::string name = pack.AssetData->Read<std::string>();

			SubmeshData submeshData = {
				.NodeName = name,
				.Transform = subHeader.Transform,
				.NodeID = subHeader.NodeID,
				.DefaultMaterialHandle = subHeader.MaterialHandle,
				.VertexCount = subHeader.VertexCount,
				.IndexCount = subHeader.IndexCount,
				.VertexOffset = subHeader.VertexOffset,
				.IndexOffset = subHeader.IndexOffset,
			};

			submeshes.push_back(submeshData);
		}

		Ref<Mesh> mesh = Ref<Mesh>::Create();

		Buffer vertices = pack.AssetData->Read<Buffer>(header.VertexCount * sizeof(Vertex3D));
		Buffer indices = pack.AssetData->Read<Buffer>(header.IndexCount * sizeof(uint32_t));

		mesh->GenerateMesh(submeshes, vertices, indices);
		info.Result(mesh);
	}

	void MeshAssetLoader::SetMaterialProperties(Ref<Material> material, const MeshImporter::MaterialData& materialData)
	{

	#define HZR_SET_MAT_PROP(mat, key, type) if(prop.Name == key) {							\
												material->Set(type, prop.Data.Data);	\
												continue;								\
											}																								
		for (auto& prop : materialData.Properties)
		{
			HZR_SET_MAT_PROP(material, "$mat.gltf.pbrMetallicRoughness.metallicFactor", "Metalness");
			HZR_SET_MAT_PROP(material, "$mat.gltf.pbrMetallicRoughness.roughnessFactor", "Roughness");
			HZR_SET_MAT_PROP(material, "$clr.diffuse", "Albedo");
		}
	}
}


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
#include <Hazard/RenderContext/ImageAssetLoader.h>

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

		Ref<Job> loadJob = Job::Create(fmt::format("Mesh load: {0}", metadata.FilePath.string()), CreateMeshFromSource, metadata);

		JobGraphInfo info = {
			.Name = "Mesh load",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "Source load", 1.0f, { loadJob  } } },
		};

		return Ref<JobGraph>::Create(info);
	}

	Ref<JobGraph> MeshAssetLoader::Save(Ref<Asset> asset, const SaveAssetSettings& settings)
	{
		Ref<Job> saveJob = Job::Create("Save", ReadMeshDataFromGPU, asset.As<Mesh>());
		Ref<Job> compileMesh = Job::Create("Mesh compile", CompileMesh, asset.As<Mesh>());

		JobGraphInfo info = {
			.Name = "Mesh save",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "Process", 0.8f, { saveJob } },
						{ "Compile", 0.2f, { compileMesh } }
			},
		};

		return Ref<JobGraph>::Create(info);
	}

	Ref<JobGraph> MeshAssetLoader::Create(const CreateAssetSettings& settings)
	{
	#if defined(HZR_PLATFORM_WINDOWS) || defined(HZR_PLATFORM_MACOS)
		CreateSettings importSettings = {};
		if (settings.Settings != nullptr)
			importSettings = *(CreateSettings*)settings.Settings;

		Ref<AssimpImporter> importer = Ref<AssimpImporter>::Create(settings.SourcePath);

		Ref<Job> preprocessJob = Job::Create(fmt::format("Mesh: {}", settings.SourcePath.string()), PreprocessDependencies, importer, importSettings);
		Ref<Job> finalize = Job::Create(fmt::format("Finalize Mesh: {}", settings.SourcePath.string()), FinalizeMesh, importer, importSettings);

		JobGraphInfo info = {
			.Name = "Mesh create",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "Preprocess", 0.1f, { preprocessJob } },
						{ "Dependency load", 0.8f, { } },
						{ "Finalize", 0.1f, { finalize } }
			},
		};

		return Ref<JobGraph>::Create(info);
	#else
		return nullptr;
	#endif
	}

	Coroutine MeshAssetLoader::CreateMeshFromSource(JobInfo info, AssetMetadata& metadata)
	{
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
		co_return;
	}


	Coroutine MeshAssetLoader::PreprocessDependencies(JobInfo info, Ref<MeshImporter> importer, const CreateSettings& settings)
	{
		Ref<Job> job = info.Current;

		importer->AddImportProgressCallback([job](float progress) mutable {
			job->Progress(progress);
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
			Ref<Job> processMeshJob = Job::Create(fmt::format("Mesh: {}", mesh.Name), ProcessMeshNode, importer, mesh);
			generateJobs.push_back(processMeshJob);
		}

		if (settings.Flags & MESH_CREATE_INCLUDE_MATERIALS)
		{
			auto materials = importer->GetMaterials();
			for (auto& material : materials)
			{
				Ref<Job> processMeshJob = Job::Create(fmt::format("Material: {}", material.Name), ProcessMaterial, importer, material, settings.MaterialPath);
				generateJobs.push_back(processMeshJob);
			}

			auto textures = importer->GetTextures();
			for (auto& texture : textures)
			{
				Ref<Job> processMeshJob = Job::Create(fmt::format("Material: {}", texture.Name), ProcessTexture, importer, texture, settings.TexturePath);
				generateJobs.push_back(processMeshJob);
			}
		}

		if (generateJobs.size() == 0)
			info.ContinueWith({ Job::Lambda("Dummy", [](JobInfo) -> Coroutine { co_return; }) });
		else info.ContinueWith(generateJobs);
		co_return;
	}

	Coroutine MeshAssetLoader::ProcessMeshNode(JobInfo info, Ref<MeshImporter> importer, const MeshImporter::MeshMetadata& mesh)
	{
		Job& jobRef = *info.Current;
		MeshImporter::MeshData data = importer->GetMeshData(mesh, [&jobRef](uint32_t current, uint32_t total) mutable {
			jobRef.Progress((float)current / (float)total);
			});

		MeshDependencyData result = {
			.MeshData = data,
		};

		info.Result(result);
		info.Current->Finish();
		co_return;
	}

	Coroutine MeshAssetLoader::ProcessMaterial(JobInfo info, Ref<MeshImporter> importer, const MeshImporter::MaterialMetadata& material, const std::filesystem::path& materialRoot)
	{
		MaterialAssetLoader::CreateSettings mat = {};

		CreateAssetSettings settings = {
			.Type = AssetType::Material,
			.AccessPath = materialRoot / (material.Name + ".hasset"),
			.Settings = &mat,
		};

		auto props = importer->GetMaterial(material.MaterialIndex);

		Ref<Material> asset = (co_await AssetManager::CreateAssetAsync<Material>(settings))[0];
		SetMaterialProperties(asset, props);

		MeshDependencyData result = {
			.Handle = asset->GetHandle(),
			.MaterialIndex = material.MaterialIndex
		};

		info.Result(result);
		info.Current->Finish();
		co_return;
	}

	Coroutine MeshAssetLoader::ProcessTexture(JobInfo info, Ref<MeshImporter> importer, const MeshImporter::TextureMetadata& texture, const std::filesystem::path& textureRoot)
	{
		static uint32_t e = 0;
		static uint32_t exit = 0;

		e++;

		auto textureData = importer->GetTextureData(texture.TextureIndex);

		ImageAssetLoader::CreateSettings imageSettings = {
			.Empty = true,
		};

		CreateAssetSettings create = {
			.Type = AssetType::Image,
			.AccessPath = textureRoot / (textureData.Name + ".hasset"),
			.Settings = &imageSettings
		};

		auto results = co_await AssetManager::CreateAssetAsync<Texture2DAsset>(create);
		Ref<Texture2DAsset> asset = results[0];

		asset->SetExtent({ textureData.Width, textureData.Height, 1 });
		asset->SetMaxMipLevels(1);
		asset->SetImageFormat(HazardRenderer::ImageFormat::RGBA);
		asset->Invalidate(textureData.ImageData);

		MeshDependencyData result = {
			.Handle = asset->GetHandle(),
			.TextureName = textureData.Name,
		};

		info.Result(result);
		textureData.ImageData.Release();

		exit++;
		HZR_CORE_INFO("{}/{}", exit, e);

		info.Current->Finish();
		co_return;
	}

	Coroutine MeshAssetLoader::FinalizeMesh(JobInfo info, Ref<MeshImporter> importer, const CreateSettings& settings)
	{
		std::vector<MeshDependencyData> results = info.Graph->GetResults<MeshDependencyData>();

		Ref<Mesh> mesh = Ref<Mesh>::Create();

		std::vector<MeshImporter::MeshData> meshData;
		std::unordered_map<uint64_t, AssetHandle> materialData;
		std::unordered_map<std::string, AssetHandle> textures;

		for (uint32_t i = 0; i < results.size(); i++)
		{
			auto result = results[i];
			if (result.MeshData.Vertices.size() > 0)
				meshData.push_back(result.MeshData);

			if (result.MaterialIndex != UINT64_MAX)
				materialData[result.MaterialIndex] = result.Handle;

			if (result.Handle)
			{
				textures[result.TextureName] = result.Handle;

				co_await AssetManager::SaveAsset(AssetManager::GetAsset<Texture2DAsset>(result.Handle));
				info.Current->Progress((float)i / (float)results.size());
			}

		}

		mesh->GenerateMesh(meshData);

		//Set materials
		for (auto& submesh : meshData)
		{
			if (!materialData.contains(submesh.MaterialIndex))
				continue;

			uint64_t nodeID = mesh->GetSubmeshNodeFromName(submesh.Name);
			AssetHandle handle = materialData[submesh.MaterialIndex];
			mesh->SetSubmeshMaterialHandle(nodeID, handle);

		}

		for (auto& material : importer->GetMaterials())
		{
			Hazard::MeshImporter::MaterialData data = importer->GetMaterial(material.MaterialIndex);
			Ref<Material> mat = AssetManager::GetAsset<Material>(materialData[material.MaterialIndex]);
			if (!mat) continue;

			for (auto& [type, textureData] : data.Textures)
				SetMaterialTextures(mat, data.Textures, textures);

			co_await AssetManager::SaveAsset(mat);
		}

		info.Result(mesh);
		info.Current->Finish();
		co_return;
	}

	Coroutine MeshAssetLoader::ReadMeshDataFromGPU(JobInfo info, Ref<Mesh> mesh)
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
		co_await cmdBuffer->Submit();

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

		result.Vertex = vertexReadback->ReadData(vertexRegion);
		result.Index = indexReadback->ReadData(indexRegion);

		info.Result(result);
		info.Current->Finish();
		co_return;
	}

	Coroutine MeshAssetLoader::CompileMesh(JobInfo info, Ref<Mesh> mesh)
	{
		struct Result
		{
			Buffer Vertex;
			Buffer Index;
		} result = info.Graph->GetResults<Result>()[0];

		auto& meshData = mesh->GetSubmeshData();

		Ref<CachedBuffer> buf = Ref<CachedBuffer>::Create();
		buf->AllowResize(true);

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
		info.Current->Finish();
		co_return;
	}


	void MeshAssetLoader::SetMaterialProperties(Ref<Material> material, const MeshImporter::MaterialData& materialData)
	{

	#define HZR_SET_MAT_PROP(mat, key, type) if(prop.Name == key) {						\
												material->SetConstant(type, prop.Data.Data);	\
												continue;								\
											}																								
		for (auto& prop : materialData.Properties)
		{
			HZR_SET_MAT_PROP(material, "$mat.gltf.pbrMetallicRoughness.metallicFactor", "Metalness");
			HZR_SET_MAT_PROP(material, "$mat.gltf.pbrMetallicRoughness.roughnessFactor", "Roughness");
			HZR_SET_MAT_PROP(material, "$clr.diffuse", "Albedo");
		}
	}
	void MeshAssetLoader::SetMaterialTextures(Ref<Material> material, std::unordered_map<MeshImporter::TextureType, MeshImporter::TextureMetadata>& textures, const std::unordered_map<std::string, AssetHandle> assets)
	{
		std::unordered_map<MeshImporter::TextureType, std::string> keys = { { MeshImporter::TextureType::Albedo, "u_Albedo" },
																			{ MeshImporter::TextureType::Diffuse, "u_Albedo" },
																			{ MeshImporter::TextureType::Normal, "u_NormalMap" }
		};

		for (auto& [type, texture] : textures)
		{
			if (keys.find(type) == keys.end()) continue;
			if (!assets.contains(texture.Name)) continue;

			std::string key = keys[type];
			Ref<Texture2DAsset> image = AssetManager::GetAsset<Texture2DAsset>(assets.at(texture.Name));
			if (image)
				material->Set(key, image);
		}
	}
}

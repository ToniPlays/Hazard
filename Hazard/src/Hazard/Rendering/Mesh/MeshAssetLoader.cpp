
#include <hzrpch.h>

#include "MeshAssetLoader.h"
#include "Importers/AssimpImporter.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Assets/AssetPack.h"

#include "Hazard/Core/Application.h"
#include "Mesh.h"

namespace Hazard
{
	Ref<JobGraph> MeshAssetLoader::Load(AssetMetadata& metadata)
	{
		HZR_PROFILE_FUNCTION();

		Ref<Job> loadJob = Ref<Job>::Create("Load", CreateMeshFromSource, metadata.Handle);

		JobGraphInfo info = {
			.Name = "Mesh load",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "Source load", 1.0f, { loadJob }} },
		};

		return Ref<JobGraph>::Create(info);
	}

	Ref<JobGraph> MeshAssetLoader::Save(Ref<Asset> asset, const SaveAssetSettings& settings)
	{
		Ref<Job> saveJob = Ref<Job>::Create("Save", ReadMeshDataFromGPU, asset.As<Mesh>());
		Ref<Job> compileMesh = Ref<Job>::Create("Mesh compile", CompileMesh, asset.As<Mesh>());

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
		MeshCreationSettings importSettings;
		if (settings.Settings != nullptr)
			importSettings = *(MeshCreationSettings*)settings.Settings;

		Ref<AssimpImporter> importer = Ref<AssimpImporter>::Create(settings.SourcePath);

		Ref<Job> preprocessJob = Ref<Job>::Create(fmt::format("Mesh: {}", settings.SourcePath.string()), PreprocessDependencies, importer);
		Ref<Job> finalize = Ref<Job>::Create(fmt::format("Finalize Mesh: {}", settings.SourcePath.string()), FinalizeMesh, importer);

		JobGraphInfo info = {
			.Name = "Mesh create",
			.Flags = JOB_GRAPH_TERMINATE_ON_ERROR,
			.Stages = { { "Preprocess", 0.6f, { preprocessJob } },
						{ "Dependency load", 0.3f, { } },
						{ "Finalize", 0.1f, { finalize } }
			},
		};

		return Ref<JobGraph>::Create(info);
	}
	void MeshAssetLoader::PreprocessDependencies(JobInfo& info, Ref<MeshImporter> importer)
	{
		Job& job = *info.Job.Raw();
		importer->AddImportProgressCallback([&job](float progress) {
			job.Progress(progress);
		});

		auto metadata = importer->GetSceneMetadata();
		auto textures = importer->GetTextures();
		auto meshes = importer->GetMeshes();
		auto animations = importer->GetAnimations();

		std::vector<Ref<Job>> generateJobs;
		generateJobs.reserve(meshes.size());

		for (uint32_t i = 0; i < meshes.size(); i++)
		{
			auto& mesh = meshes[i];
			Ref<Job> processMeshJob = Ref<Job>::Create(fmt::format("Node: {}", mesh.Name), ProcessMeshNode, importer, mesh);
			generateJobs.push_back(processMeshJob);
		}

		info.ParentGraph->ContinueWith(generateJobs);
	}
	void MeshAssetLoader::ProcessMeshNode(JobInfo& info, Ref<MeshImporter> importer, const MeshImporter::MeshMetadata& mesh)
	{
		Job& jobRef = *info.Job;
		MeshImporter::MeshData data = importer->GetMeshData(mesh, [&jobRef](uint32_t current, uint32_t total) mutable {
			jobRef.Progress((float)current / (float)total);
		});

		info.Job->SetResult(data);
	}
	void MeshAssetLoader::FinalizeMesh(JobInfo& info, Ref<MeshImporter> importer)
	{
		auto results = info.ParentGraph->GetResults<MeshImporter::MeshData>();

		Ref<Mesh> mesh = Ref<Mesh>::Create();
		mesh->GenerateMesh(results);

		info.Job->SetResult(mesh);
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

		for (auto& [uid, submesh] : mesh->GetSubmeshData())
		{
			BufferCopyRegion vertexRegion = {
				.Size = submesh.VertexCount * sizeof(Vertex3D),
				.Offset = submesh.VertexOffset,
			};

			BufferCopyRegion indexRegion = {
				.Size = submesh.IndexCount * sizeof(uint32_t),
				.Offset = submesh.IndexOffset,
			};

			cmdBuffer->CopyBufferToBuffer(mesh->GetVertexBuffer(uid), vertexRegion, vertexReadback, vertexRegion);
			cmdBuffer->CopyBufferToBuffer(mesh->GetIndexBuffer(uid), indexRegion, indexReadback, indexRegion);
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

			result.Vertex = vertexReadback->ReadData(vertexRegion);
			result.Index = indexReadback->ReadData(indexRegion);

			info.Job->SetResult(result);
			info.ParentGraph->Continue();
		});

		info.ParentGraph->Halt();
	}
	void MeshAssetLoader::CompileMesh(JobInfo& info, Ref<Mesh> mesh)
	{
		struct Result
		{
			Buffer Vertex;
			Buffer Index;
		} result = info.ParentGraph->GetResult<Result>();

		auto meshData = mesh->GetSubmeshData();

		uint64_t nodeNameLength = 0;
		for (auto& [uid, submesh] : meshData)
			nodeNameLength += submesh.NodeName.length() + sizeof(uint64_t);

		Buffer buffer;
		buffer.Allocate(sizeof(MeshFileHeader) + sizeof(SubmeshHeader) * meshData.size() + nodeNameLength + result.Vertex.Size + result.Index.Size);
		CachedBuffer buf(buffer.Data, buffer.Size);

		MeshFileHeader meshHeader = {
			.SubmeshCount = meshData.size(),
			.VertexCount = result.Vertex.Size / sizeof(Vertex3D),
			.IndexCount = result.Index.Size / sizeof(uint32_t),
		};

		buf.Write(meshHeader);

		uint32_t progress = 0;
		for (auto& [uid, submesh] : meshData)
		{
			SubmeshHeader header = {
				.NodeID = uid,
				.VertexCount = submesh.VertexCount,
				.IndexCount = submesh.IndexCount,
				.VertexOffset = submesh.VertexOffset,
				.IndexOffset = submesh.IndexOffset,
			};

			buf.Write(header);
			buf.Write(submesh.NodeName);

			progress++;
			info.Job->Progress((float)progress / (float)meshData.size());
		}

		buf.Write(result.Vertex);
		buf.Write(result.Index);

		result.Vertex.Release();
		result.Index.Release();

		info.Job->SetResult(buffer);
	}
	void MeshAssetLoader::CreateMeshFromSource(JobInfo& info, AssetHandle handle)
	{
		AssetMetadata& metadata = AssetManager::GetMetadata(handle);

		if (!File::Exists(metadata.FilePath))
			throw JobException("File does not exist");

		CachedBuffer buffer = File::ReadBinaryFile(metadata.FilePath);
		AssetPack pack = {};
		pack.FromBuffer(buffer);

		CachedBuffer data(pack.AssetData.Data, pack.AssetData.Size);
		MeshFileHeader header = data.Read<MeshFileHeader>();

		std::unordered_map<uint64_t, SubmeshData> submeshes;

		for (uint32_t i = 0; i < header.SubmeshCount; i++)
		{
			SubmeshHeader subHeader = data.Read<SubmeshHeader>();

			SubmeshData submeshData = {
				.NodeName = data.Read<std::string>(),
				.NodeID = subHeader.NodeID,
				.VertexCount = subHeader.VertexCount,
				.IndexCount = subHeader.IndexCount,
				.VertexOffset = subHeader.VertexOffset,
				.IndexOffset = subHeader.IndexOffset,
			};

			submeshes[submeshData.NodeID] = submeshData;
		}

		Ref<Mesh> mesh = Ref<Mesh>::Create();

		Buffer vertices = data.Read<Buffer>(header.VertexCount * sizeof(Vertex3D));
		Buffer indices = data.Read<Buffer>(header.IndexCount * sizeof(uint32_t));
		mesh->GenerateMesh(submeshes, vertices, indices);

		info.Job->SetResult(mesh);
	}
}

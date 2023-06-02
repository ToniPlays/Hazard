
#include <hzrpch.h>
#include "WorldDeserializer.h"
#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Physics/PhysicsCommand.h"

namespace Hazard
{
	void WorldDeserializer::ProcessWorld(Ref<Job> job, std::filesystem::path file)
	{
		Ref<World> world = Ref<World>::Create(file);

		WorldDeserializer deserializer;

		YAML::Node root = YAML::LoadFile(file.string());
		if (!root["World"])
			return;

		//Loop entities
		auto entities = root["Entities"];
		size_t index = 0;
		if (entities)
		{
			for (size_t i = entities.size(); i > 0; --i)
			{
				auto node = entities[i - 1];
				UID uid = node["Entity"].as<uint64_t>();
				Entity entity = world->CreateEntity(uid, "");

				//Deserialize components
				deserializer.TryDeserializeComponent<TagComponent>("TagComponent", entity, node);
				deserializer.TryDeserializeComponent<TransformComponent>("TransformComponent", entity, node);
				deserializer.TryDeserializeComponent<CameraComponent>("CameraComponent", entity, node);

				deserializer.TryDeserializeComponent<ScriptComponent>("ScriptComponent", entity, node);

				deserializer.TryDeserializeComponent<SkyLightComponent>("SkyLightComponent", entity, node);
				deserializer.TryDeserializeComponent<DirectionalLightComponent>("DirectionalLightComponent", entity, node);
				deserializer.TryDeserializeComponent<PointLightComponent>("PointLightComponent", entity, node);

				deserializer.TryDeserializeComponent<MeshComponent>("MeshComponent", entity, node);
				deserializer.TryDeserializeComponent<SpriteRendererComponent>("SpriteRendererComponent", entity, node);

				deserializer.TryDeserializeComponent<Rigidbody2DComponent>("Rigidbody2DComponent", entity, node);
				deserializer.TryDeserializeComponent<BoxCollider2DComponent>("BoxCollider2DComponent", entity, node);
				deserializer.TryDeserializeComponent<CircleCollider2DComponent>("CircleCollider2DComponent", entity, node);

				index++;
				job->Progress((float)index / (float)entities.size());
			}
		}

		AssetMetadata& metadata = AssetManager::GetMetadata(AssetManager::GetHandleFromKey(file.string()));
		metadata.Type = AssetType::World;
		world->m_Handle = metadata.Handle;

		world->IncRefCount();
		job->GetStage()->SetResult(world);
		HZR_CORE_INFO("Loaded world {}", metadata.Key);
	}
	static void LoadWorldAsset(Ref<Job> job, AssetHandle handle)
	{
		HZR_CORE_INFO("World loading requires asset {0}", handle);
		Ref<Asset> asset = AssetManager::GetAsset<Asset>(handle);
		if (asset)
			asset->IncRefCount();
	}
	static void DeserializeYAMLWorld(Ref<Job> job, const std::filesystem::path& path)
	{
		YAML::Node root = YAML::LoadFile(path.string());

		if (!root["World"])
			job->GetStage()->SetResult(nullptr);

		auto entities = root["Entities"];

		if (!entities) return;

		std::vector<AssetHandle> assetsToLoad;

		size_t index = 0;
		for (size_t i = entities.size(); i > 0; --i)
		{
			auto node = entities[i - 1];
			//UID uid = node["Entity"].as<uint64_t>();

			//TryDeserializeComponent<ScriptComponent>("ScriptComponent", entity, node);

			//TryDeserializeComponent<SkyLightComponent>("SkyLightComponent", entity, node);
			//TryDeserializeComponent<DirectionalLightComponent>("DirectionalLightComponent", entity, node);
			//TryDeserializeComponent<PointLightComponent>("PointLightComponent", entity, node);

			WorldDeserializer::FetchAssetsToLoad<MeshComponent>("MeshComponent", node, assetsToLoad);
			WorldDeserializer::FetchAssetsToLoad<SpriteRendererComponent>("SpriteRendererComponent", node, assetsToLoad);

			index++;
			job->Progress((float)index / (float)entities.size());
		}

		std::vector<Ref<Job>> jobs;
		jobs.reserve(assetsToLoad.size());

		Ref<GraphStage> nextStage = job->GetJobGraph()->GetNextStage();

		for (auto& handle : assetsToLoad)
		{
			Ref<Job> job = Ref<Job>::Create(LoadWorldAsset, handle);
			jobs.push_back(job);
		}
		nextStage->QueueJobs({ jobs });
	}

	Ref<JobGraph> WorldDeserializer::DeserializeEditor(const std::filesystem::path& file)
	{
		HZR_PROFILE_FUNCTION();

		//World loading 3 stages, process, load assets and create world
		Ref<Job> loadingJob = Ref<Job>::Create(DeserializeYAMLWorld, file);
		loadingJob->SetJobTag("Load");
		loadingJob->SetJobName(fmt::format("Load {}", file.string()));

		Ref<Job> worldProcessing = Ref<Job>::Create(ProcessWorld, file);

		Ref<JobGraph> graph = Ref<JobGraph>::Create("Deserialize world", 3);
		graph->GetStage(0)->QueueJobs({ loadingJob });
		graph->GetStage(0)->SetWeight(0.1f);
		graph->GetStage(1)->SetWeight(0.8f);
		graph->GetStage(2)->SetWeight(0.1f);
		graph->GetStage(2)->QueueJobs({ worldProcessing });

		return graph;
	}
	template<typename T>
	void WorldDeserializer::TryDeserializeComponent(const char* key, Entity entity, YAML::Node node)
	{
		if (!node[key]) 
			return;

		Deserialize<T>(entity, node[key]);
	}
}

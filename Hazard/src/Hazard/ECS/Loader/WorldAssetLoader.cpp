
#include <hzrpch.h>
#include "WorldAssetLoader.h"
#include "WorldDeserializer.h"
#include "WorldSerializer.h"

#include "Hazard/Core/Application.h"

namespace Hazard
{
	LoadType WorldAssetLoader::Load(AssetMetadata& metadata, Ref<Asset>& asset, uint32_t flags)
	{
		HZR_PROFILE_FUNCTION();
		if (!File::Exists(metadata.Path))
			return LoadType::Failed;

		WorldDeserializer deserializer;
		asset = deserializer.DeserializeEditor(metadata.Path, flags | AssetManagerFlags_CanAsync);
		//Wait for assets to be loaded
		//for (auto& promise : deserializer.GetPromises())
		//	promise.Promise.Wait();

		return LoadType::Source;
	}
	/*
	Ref<JobGraph> WorldAssetLoader::LoadAsync(AssetMetadata& metadata, uint32_t flags)
	{
		return nullptr;
		return Application::Get().SubmitJob<Ref<Asset>>("WorldAsync", [meta = metadata, flags](JobSystem* system, Job* job) -> size_t {

			if (!File::Exists(meta.Path))
				return (size_t)LoadType::Failed;

			WorldDeserializer deserializer;
			deserializer.SetProgressHandler([job](Entity& entity, size_t index, size_t total) {
				JOB_PROGRESS(job, index, total);
#ifdef HZR_DEBUG
				std::this_thread::sleep_for(100ms);
#endif
				});

			Ref<World> world = deserializer.DeserializeEditor(meta.Path, flags);
			world->m_Promises = deserializer.GetPromises();

			*job->Value<Ref<World>>() = std::move(world);
			return (size_t)LoadType::Source;
			});
	}
	*/
	bool WorldAssetLoader::Save(Ref<Asset>& asset)
	{
        auto world = asset.As<World>();
        
        WorldSerializer serializer(world);
        serializer.SerializeEditor(world->GetWorldFile());
        
        return true;
	}
	/*Ref<JobGraph> WorldAssetLoader::SaveAsync(Ref<Asset>& asset)
	{
		HZR_CORE_ASSERT(false, "TODO");
		return nullptr;
	}*/
}

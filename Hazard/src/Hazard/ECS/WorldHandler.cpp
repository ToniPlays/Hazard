
#include <hzrpch.h>
#include "WorldHandler.h"
#include "Loader/WorldDeserializer.h"
#include "Loader/WorldSerializer.h"
#include "Hazard/Rendering/HRenderer.h"
#include "Hazard/ECS/Loader/WorldAssetLoader.h"
#include "HazardScript.h"
#include "Hazard/Math/Time.h"

#include "Hazard/Core/Application.h"

namespace Hazard {

	WorldHandler::WorldHandler(EntityComponentCreateInfo* info) : Module::Module("World handler")
	{
		m_World = Ref<World>::Create(info->StartupFile);
		AssetManager::RegisterLoader<WorldAssetLoader>(AssetType::World);
		SetActive(true);
	}
	WorldHandler::~WorldHandler() {}

	void WorldHandler::Init()
	{

	}

	void WorldHandler::Close()
	{

	}

	void WorldHandler::Update()
	{
		HZR_PROFILE_FUNCTION();
		if (!(m_Flags & WorldFlags_UpdateScript)) return;

		Ref<World> world = m_World;
		auto view = world->GetEntitiesWith<ScriptComponent>();

		float delta = (float)Time::s_DeltaTime;
		void* params[] = { &delta };

		for (auto& entity : view)
		{
			Entity e = { entity, world.Raw() };
			auto& sc = e.GetComponent<ScriptComponent>();
			if (!e.ReceivesUpdate() || !sc.Active || !sc.m_Handle) continue;

			sc.m_Handle->TryInvoke("OnUpdate(single)", params);
		}
	}

	void WorldHandler::OnBegin()
	{
		HZR_PROFILE_FUNCTION();
		Ref<World> world = m_World;
		auto view = world->GetEntitiesWith<ScriptComponent>();

		for (auto& entity : view)
		{
			Entity e = { entity, world.Raw() };
			auto& sc = e.GetComponent<ScriptComponent>();
			if (sc.m_Handle)
			{
				sc.m_Handle->SetLive(true);
				sc.m_Handle->TryInvoke("OnCreate()", nullptr);
			}
		}
	}

	void WorldHandler::OnEnd()
	{
		HZR_PROFILE_FUNCTION();
		auto view = m_World->GetEntitiesWith<ScriptComponent>();

		for (auto& entity : view) {
			Entity e = { entity, m_World.Raw() };
			auto& sc = e.GetComponent<ScriptComponent>();
			if (sc.m_Handle) {
				sc.m_Handle->SetLive(false);
				sc.m_Handle->TryInvoke("OnDestroy()", nullptr);
			}
		}
	}

	bool WorldHandler::LoadWorld(const std::filesystem::path& file, Serialization type)
	{
		HZR_PROFILE_FUNCTION();
		if (File::Exists(file))
		{
			AssetHandle handle = AssetManager::GetHandleFromFile(file);
			HZR_ASSERT(handle != INVALID_ASSET_HANDLE, "World handle is invalid");
			m_World = AssetManager::GetAsset<World>(handle);
			return true;
		}

		m_World = Ref<World>::Create("");
		m_World->SetName("New World");

		Entity entity = m_World->CreateEntity("Camera");
		m_World->CreateEntity("Entity 1");
		entity.AddComponent<CameraComponent>();

		return false;
	}
	JobPromise WorldHandler::LoadWorldAsync(const std::filesystem::path& file, Serialization type, WorldAsyncPromises* promises)
	{
		HZR_PROFILE_FUNCTION();
		if (File::Exists(file))
		{
			AssetHandle handle = AssetManager::GetHandleFromFile(file);
			HZR_ASSERT(handle != INVALID_ASSET_HANDLE, "World handle is invalid");

			auto promise = AssetManager::GetAssetAsync<World>(handle);
			promise.Wait();

			auto waitPromise = promise.Then([handle, type](JobSystem* system, Job* job) -> size_t {
				Job* dependency = system->GetJob(job->Dependency);
				Application::GetModule<WorldHandler>().SetWorld(*dependency->Value<Ref<World>>());
				return 0;
				});

			if (promises)
			{
				Ref<World> loaded = *promise.Value();
				promises->WorldPromise = std::move(promise);
				promises->AssetPromises.reserve(loaded->GetPromises().size());

				for (auto& [type, p] : loaded->GetPromises())
					promises->AssetPromises.emplace_back(WorldAsyncAssetPromise(type, p));
			}

			return waitPromise;

		}
		m_World = Ref<World>::Create("");
		m_World->SetName("New World");

		Entity entity = m_World->CreateEntity("Camera");
		m_World->CreateEntity("Entity 1");
		entity.AddComponent<CameraComponent>();

		return TypedJobPromise<Ref<World>>();
	}
}

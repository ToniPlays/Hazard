
#include <hzrpch.h>
#include "WorldHandler.h"
#include "Loader/WorldDeserializer.h"
#include "Loader/WorldSerializer.h"
#include "Hazard/Rendering/HRenderer.h"
#include "Hazard/ECS/Loader/WorldAssetLoader.h"
#include "Hazard/Scripting/ScriptEngine.h"
#include "HazardScript.h"
#include "Hazard/Math/Time.h"

#include "Hazard/Core/Application.h"

namespace Hazard
{

	WorldHandler::WorldHandler(EntityComponentCreateInfo* info) : Module::Module("World handler")
	{
		AssetManager::RegisterLoader<WorldAssetLoader>(AssetType::World);

		if (info->StartupFile.empty())
			m_World = Ref<World>::Create("0");
		else
		{
			AssetHandle handle = AssetManager::GetHandleFromKey(info->StartupFile.string());
			if (handle != INVALID_ASSET_HANDLE)
				m_World = AssetManager::GetAsset<World>(handle);
			else m_World = Ref<World>::Create("0");
		}
		SetActive(true);
	}
	WorldHandler::~WorldHandler()
	{

	}

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

		auto view = m_World->GetEntitiesWith<ScriptComponent>();
		float delta = (float)Time::s_DeltaTime;

		for (auto& entity : view)
		{
			Entity e = { entity, m_World.Raw() };
			auto& sc = e.GetComponent<ScriptComponent>();
			if (!e.ReceivesUpdate() || !sc.Active || !sc.m_Handle) continue;

			sc.m_Handle->TryInvoke("OnUpdate", delta);
		}
	}

	void WorldHandler::OnBegin()
	{
		HZR_PROFILE_FUNCTION();
		auto view = m_World->GetEntitiesWith<ScriptComponent>();

		for (auto& entity : view)
		{
			Entity e = { entity, m_World.Raw() };
			auto& sc = e.GetComponent<ScriptComponent>();
			if (!sc.m_Handle) continue;

			sc.m_Handle->SetLive(true);
			sc.m_Handle->TryInvoke("OnCreate");
		}
	}

	void WorldHandler::OnEnd()
	{
		HZR_PROFILE_FUNCTION();
		auto view = m_World->GetEntitiesWith<ScriptComponent>();

		for (auto& entity : view)
		{
			Entity e = { entity, m_World.Raw() };
			auto& sc = e.GetComponent<ScriptComponent>();
			if (sc.m_Handle)
			{
				sc.m_Handle->SetLive(false);
				sc.m_Handle->TryInvoke("OnDestroy");
			}
		}
	}

	bool WorldHandler::LoadWorld(AssetHandle handle)
	{
		HZR_PROFILE_FUNCTION();
		if (handle != INVALID_ASSET_HANDLE)
		{
			m_World = AssetManager::GetAsset<World>(handle);
			return m_World;
		}

		m_World = Ref<World>::Create("0");

		Entity entity = m_World->CreateEntity("Camera");
		m_World->CreateEntity("Entity 1");
		entity.AddComponent<CameraComponent>();

		return false;
	}
}

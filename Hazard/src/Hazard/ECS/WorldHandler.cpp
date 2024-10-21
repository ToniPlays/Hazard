
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
        m_World = AssetManager::CreateAsset<World>(CreateAssetSettings());
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
		if (m_Flags & WorldFlags_UpdateScript)
		{

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
	}

	void WorldHandler::OnBegin()
	{
		HZR_PROFILE_FUNCTION();

		auto tcView = m_World->GetEntitiesWith<TransformComponent>();
		for (auto& entity : tcView)
		{
			Entity e = { entity, m_World.Raw() };
			e.UpdateWorldTransforms();
		}

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
}

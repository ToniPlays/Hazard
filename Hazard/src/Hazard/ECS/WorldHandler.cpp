
#include <hzrpch.h>
#include "WorldHandler.h"
#include "Loader/WorldDeserializer.h"
#include "Loader/WorldSerializer.h"
#include "Hazard/Rendering/HRenderer.h"
#include "HazardScript.h"
#include "Hazard/Math/Time.h"

namespace Hazard {

	WorldHandler::WorldHandler(EntityComponentCreateInfo* info) : Module::Module("World handler")
	{
		m_World = Ref<World>::Create(info->StartupFile);
		SetActive(true);
	}
	WorldHandler::~WorldHandler() {}

	void WorldHandler::Init()
	{
		LoadWorld(m_World->GetWorldFile(), Serialization::Editor);
	}

	void WorldHandler::Close()
	{

	}

	void WorldHandler::Update()
	{
		HZR_PROFILE_FUNCTION();
		if (!(m_Flags & WorldFlags_UpdateScript)) return;
		auto& view = m_World->GetEntitiesWith<ScriptComponent>();

		float delta = (float)Time::s_DeltaTime;
		void* params[] = { &delta };

		for (auto& entity : view) {
			Entity e = { entity, m_World.Raw() };
			auto& sc = e.GetComponent<ScriptComponent>();
			if (!e.ReceivesUpdate() || !sc.Active || !sc.m_Handle) continue;

			sc.m_Handle->TryInvoke("OnUpdate(single)", params);
		}
	}

	void WorldHandler::OnBegin()
	{
		HZR_PROFILE_FUNCTION();
		auto& view = m_World->GetEntitiesWith<ScriptComponent>();

		for (auto& entity : view) {
			Entity e = { entity, m_World.Raw() };
			auto& sc = e.GetComponent<ScriptComponent>();
			if (sc.m_Handle) {
				sc.m_Handle->SetLive(true);
				sc.m_Handle->TryInvoke("OnCreate()", nullptr);
			}
		}
	}

	void WorldHandler::OnEnd()
	{
		HZR_PROFILE_FUNCTION();
		auto& view = m_World->GetEntitiesWith<ScriptComponent>();

		for (auto& entity : view) {
			Entity e = { entity, m_World.Raw() };
			auto& sc = e.GetComponent<ScriptComponent>();
			if (sc.m_Handle) {
				sc.m_Handle->SetLive(false);
			}
		}
	}

	bool WorldHandler::LoadWorld(const std::filesystem::path& file, Serialization type)
	{
		if (File::Exists(file)) {
			if (type == Serialization::Editor) {
				WorldDeserializer deserializer;
				m_World = deserializer.DeserializeEditor(file);
				return true;
			}
		}

		m_World = Ref<World>::Create("");
		m_World->SetName("New World");

		Entity entity = m_World->CreateEntity("Camera");
		m_World->CreateEntity("Entity 1");
		entity.AddComponent<CameraComponent>();

		return false;
	}
}

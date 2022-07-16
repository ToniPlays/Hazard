
#include <hzrpch.h>
#include "WorldHandler.h"
#include "Loader/WorldDeserializer.h"
#include "Loader/WorldSerializer.h"
#include "Hazard/Rendering/HRenderer.h"


namespace Hazard {

	WorldHandler::WorldHandler(EntityComponentCreateInfo* info) : Module::Module("World handler") 
	{
		LoadWorld(info->StartupFile, Serialization::Editor);
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
	}

	void WorldHandler::Render()
	{
		HZR_PROFILE_FUNCTION();
		//Submit everything for drawing
		auto& view = m_World->GetEntitiesWith<SpriteRendererComponent>();
		for (auto& entity : view) {
			Entity e = { entity, m_World.Raw() };
			auto& tc = e.GetComponent<TransformComponent>();
			auto& sc = e.GetComponent<SpriteRendererComponent>();

			HRenderer::SubmitSprite(tc, sc);
		}
	}

	bool WorldHandler::LoadWorld(const std::string& file, Serialization type)
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
		m_World->CreateEntity("Entity 2");
		entity.AddComponent<CameraComponent>();

		return false;
	}
}

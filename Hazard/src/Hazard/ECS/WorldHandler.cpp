
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
		{
			//Submit sky lights for drawing
			auto& view = m_World->GetEntitiesWith<SkyLightComponent>();
			for (auto& entity : view) {
				Entity e = { entity, m_World.Raw() };
				HRenderer::SubmitSkyLight(e.GetComponent<SkyLightComponent>());
			}
		}
		{
			//Submit directional lights for drawing
			auto& view = m_World->GetEntitiesWith<DirectionalLightComponent>();
			for (auto& entity : view) {
				Entity e = { entity, m_World.Raw() };
				HRenderer::SubmitDirectionalLight(e.GetComponent<TransformComponent>(), e.GetComponent<DirectionalLightComponent>());
			}
		}
		{
			//Submit point lights for drawing
			auto& view = m_World->GetEntitiesWith<PointLightComponent>();
			for (auto& entity : view) {
				Entity e = { entity, m_World.Raw() };
				HRenderer::SubmitPointLight(e.GetComponent<TransformComponent>(), e.GetComponent<PointLightComponent>());
			}
		}
		{
			//Submit everything for drawing
			auto& view = m_World->GetEntitiesWith<SpriteRendererComponent>();
			for (auto& entity : view) {
				Entity e = { entity, m_World.Raw() };
				HRenderer::SubmitSprite(e.GetComponent<TransformComponent>(), e.GetComponent<SpriteRendererComponent>());
			}
		}
		{
			auto& view = m_World->GetEntitiesWith<BatchComponent>();
			for (auto& entity : view) {
				Entity e = { entity, m_World.Raw() };
				auto& tc = e.GetComponent<TransformComponent>();
				auto& bc = e.GetComponent<BatchComponent>();

				for (uint32_t x = 0; x < bc.Size; x++)
				{
					for (uint32_t y = 0; y < bc.Size; y++)
					{
						glm::mat4 transform = Math::ToTransformMatrix({ x, y, 0.0f });

						HRenderer::SubmitQuad(transform, { bc.Tint.r, bc.Tint.g, bc.Tint.b, bc.Tint.a });
					}
				}
			}
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

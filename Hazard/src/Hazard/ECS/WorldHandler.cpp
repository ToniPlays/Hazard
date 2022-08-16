
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

		float delta = Time::s_DeltaTime;
		void* params[] = { &delta };

		for (auto& entity : view) {
			Entity e = { entity, m_World.Raw() };
			auto& sc = e.GetComponent<ScriptComponent>();
			if (!e.ReceivesUpdate() || !sc.Active || !sc.m_Handle) continue;

			sc.m_Handle->TryInvoke("OnUpdate(single)", params);
		}
	}

	void WorldHandler::Render()
	{
		if(!(m_Flags & WorldFlags_Render)) return;
		HZR_PROFILE_FUNCTION();
		{
			//Submit sky lights for drawing
			auto& view = m_World->GetEntitiesWith<SkyLightComponent>();
			for (auto& entity : view) {
				Entity e = { entity, m_World.Raw() };
				auto& sky = e.GetComponent<SkyLightComponent>();
				if (!e.IsVisible() || !sky.Active) continue;
				HRenderer::SubmitSkyLight(sky);
			}
		}
		{
			//Submit directional lights for drawing
			auto& view = m_World->GetEntitiesWith<DirectionalLightComponent>();
			for (auto& entity : view) {
				Entity e = { entity, m_World.Raw() };
				auto& dl = e.GetComponent<DirectionalLightComponent>();
				if (!e.IsVisible() || dl.Active) continue;
				HRenderer::SubmitDirectionalLight(e.GetComponent<TransformComponent>(), dl);
			}
		}
		{
			//Submit point lights for drawing
			auto& view = m_World->GetEntitiesWith<PointLightComponent>();
			for (auto& entity : view) {
				Entity e = { entity, m_World.Raw() };
				auto& pl = e.GetComponent<PointLightComponent>();
				if (!e.IsVisible() || !pl.Active) continue;
				HRenderer::SubmitPointLight(e.GetComponent<TransformComponent>(), pl);
			}
		}
		{
			//Submit everything for drawing
			auto& view = m_World->GetEntitiesWith<SpriteRendererComponent>();
			for (auto& entity : view) {
				Entity e = { entity, m_World.Raw() };
				auto& sr = e.GetComponent<SpriteRendererComponent>();
				if (!e.IsVisible() || !sr.Active) continue;
				HRenderer::SubmitSprite(e.GetComponent<TransformComponent>(), sr);
			}
		}
		{
			//Submit Meshes for drawing
			auto& view = m_World->GetEntitiesWith<MeshComponent>();
			for (auto& entity : view) {
				Entity e = { entity, m_World.Raw() };
				auto& mc = e.GetComponent<MeshComponent>();
				if (!e.IsVisible() || !mc.Active) continue;
				HRenderer::SubmitMesh(e.GetComponent<TransformComponent>(), mc);
			}
		}
		{
			auto& view = m_World->GetEntitiesWith<BatchComponent>();
			for (auto& entity : view) {
				Entity e = { entity, m_World.Raw() };
				if (!e.IsVisible()) continue;
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

	void WorldHandler::OnBegin()
	{
		HZR_PROFILE_FUNCTION();
		auto& view = m_World->GetEntitiesWith<ScriptComponent>();

		for (auto& entity : view) {
			Entity e = { entity, m_World.Raw() };
			auto& sc = e.GetComponent<ScriptComponent>();
			if (sc.m_Handle)
				sc.m_Handle->TryInvoke("OnCreate()", nullptr);
		}
	}

	void WorldHandler::OnEnd()
	{
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

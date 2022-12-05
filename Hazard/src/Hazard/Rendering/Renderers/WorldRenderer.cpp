
#include <hzrpch.h>
#include "WorldRenderer.h"
#include "Hazard/Rendering/HRenderer.h"
#include "Hazard/ECS/Entity.h"

using namespace HazardRenderer;

namespace Hazard 
{
	WorldRenderer::WorldRenderer(WorldRendererSpec* spec) : m_Spec(*spec)
	{
		m_TargetWorld = Ref<World>::Create("New world");
	}
	void WorldRenderer::Submit()
	{
		HZR_PROFILE_FUNCTION();
		HRenderer::SubmitWorldRenderer(this);
	}
	void WorldRenderer::Render()
	{
		HZR_PROFILE_FUNCTION();
		HZR_TIMED_FUNCTION();
		{
			//Submit sky lights for drawing
			auto view = m_TargetWorld->GetEntitiesWith<SkyLightComponent>();
			for (auto& entity : view) {
				Entity e = { entity, m_TargetWorld.Raw() };
				auto& sky = e.GetComponent<SkyLightComponent>();
				if (!e.IsVisible() || !sky.Active) continue;
				HRenderer::SubmitSkyLight(sky);
			}
		}
		{
			//Submit directional lights for drawing
			auto view = m_TargetWorld->GetEntitiesWith<DirectionalLightComponent>();
			for (auto& entity : view) {
				Entity e = { entity, m_TargetWorld.Raw() };
				auto& dl = e.GetComponent<DirectionalLightComponent>();
				if (!e.IsVisible() || !dl.Active) continue;
				HRenderer::SubmitDirectionalLight(e.GetComponent<TransformComponent>(), dl);
			}
		}
		{
			//Submit point lights for drawing
			auto view = m_TargetWorld->GetEntitiesWith<PointLightComponent>();
			for (auto& entity : view) {
				Entity e = { entity, m_TargetWorld.Raw() };
				auto& pl = e.GetComponent<PointLightComponent>();
				if (!e.IsVisible() || !pl.Active) continue;
				HRenderer::SubmitPointLight(e.GetComponent<TransformComponent>(), pl);
			}
		}
		{
			//Submit Sprites for drawing
			auto view = m_TargetWorld->GetEntitiesWith<SpriteRendererComponent>();
			for (auto& entity : view) {
				Entity e = { entity, m_TargetWorld.Raw() };
				auto& sr = e.GetComponent<SpriteRendererComponent>();
				if (!e.IsVisible() || !sr.Active) continue;
				HRenderer::SubmitSprite(e.GetComponent<TransformComponent>(), sr);
			}
		}
		{
			//Submit Meshes for drawing
			auto view = m_TargetWorld->GetEntitiesWith<MeshComponent>();
			for (auto& entity : view) {
				Entity e = { entity, m_TargetWorld.Raw() };
				auto& mc = e.GetComponent<MeshComponent>();
				if (!e.IsVisible() || !mc.Active) continue;
				HRenderer::SubmitMesh(e.GetComponent<TransformComponent>(), mc);
			}
		}
	}
}

#include <hzrpch.h>
#include "WorldRenderer.h"
#include "Hazard/Rendering/HRenderer.h"
#include "Hazard/ECS/Entity.h"

using namespace HazardRenderer;

namespace Hazard 
{
	WorldRenderer::WorldRenderer(WorldRendererSpec* spec) : m_Spec(*spec)
	{
		HZR_ASSERT(spec->TargetWorld, "World renderer requires target world");
		m_TargetWorld = spec->TargetWorld;
	}

	void WorldRenderer::SetTargetWorld(Ref<World> world)
	{
		HZR_CORE_ASSERT(world, "Target world cannot be nullptr");
		m_TargetWorld = world;
	}

	void WorldRenderer::SubmitCamera(const WorldCameraData& camera)
	{
		m_CameraData.push_back(camera);
		if (m_CameraData.size() <= m_CameraDescriptors.size()) return;

		//Create descriptor for current camera
		Ref<Pipeline> pbrShader = ShaderLibrary::GetPipeline("PBR_Static");
		auto layout = pbrShader->GetSpecifications().SetLayouts[0];

		DescriptorSetCreateInfo setInfo = {
			.DebugName = "WorldDescriptor",
			.Set = 0,
			.pLayout = &layout,
		};

		m_CameraDescriptors.push_back(DescriptorSet::Create(&setInfo));
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
			for (auto& entity : view) 
			{
				Entity e = { entity, m_TargetWorld.Raw() };
				auto& sky = e.GetComponent<SkyLightComponent>();
				if (!e.IsVisible() || !sky.Active) continue;
				HRenderer::SubmitSkyLight(sky);
			}
		}
		{
			//Submit directional lights for drawing
			auto view = m_TargetWorld->GetEntitiesWith<DirectionalLightComponent>();
			for (auto& entity : view) 
			{
				Entity e = { entity, m_TargetWorld.Raw() };
				auto& dl = e.GetComponent<DirectionalLightComponent>();
				if (!e.IsVisible() || !dl.Active) continue;
				HRenderer::SubmitDirectionalLight(e.GetComponent<TransformComponent>(), dl);
			}
		}
		{
			//Submit point lights for drawing
			auto view = m_TargetWorld->GetEntitiesWith<PointLightComponent>();
			for (auto& entity : view) 
			{
				Entity e = { entity, m_TargetWorld.Raw() };
				auto& pl = e.GetComponent<PointLightComponent>();
				if (!e.IsVisible() || !pl.Active) continue;
				HRenderer::SubmitPointLight(e.GetComponent<TransformComponent>(), pl);
			}
		}
		{
			//Submit Sprites for drawing
			auto view = m_TargetWorld->GetEntitiesWith<SpriteRendererComponent>();
			for (auto& entity : view)
            {
				Entity e = { entity, m_TargetWorld.Raw() };
				auto& sr = e.GetComponent<SpriteRendererComponent>();
				if (!e.IsVisible() || !sr.Active) continue;
				HRenderer::SubmitSprite(e.GetComponent<TransformComponent>(), sr, (uint32_t)e.GetHandle() + 1);
			}
		}
		{
			//Submit Meshes for drawing
			auto view = m_TargetWorld->GetEntitiesWith<MeshComponent>();
			for (auto& entity : view)
            {
				Entity e = { entity, m_TargetWorld.Raw() };
				auto& mc = e.GetComponent<MeshComponent>();
				if (!e.IsVisible() || !mc.Active) continue;
				HRenderer::SubmitMesh(e.GetComponent<TransformComponent>(), mc, (uint32_t)e.GetHandle() + 1);
			}
		}
	}
}

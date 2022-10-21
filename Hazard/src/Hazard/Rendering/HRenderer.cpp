
#include <hzrpch.h>
#include "HRenderer.h"
#include "Renderers/WorldRenderer.h"
#include "Hazard/Assets/AssetManager.h"

namespace Hazard
{
	using namespace HazardRenderer;

	void HRenderer::SubmitWorldRenderer(WorldRenderer* renderer)
	{
		HZR_PROFILE_FUNCTION();
		RendererDrawList list = {};
		list.WorldRenderer = renderer;
		s_Engine->GetDrawLists().push_back(list);
	}

	void HRenderer::SubmitSprite(const TransformComponent& transform, const SpriteRendererComponent& spriteRenderer)
	{
		HZR_PROFILE_FUNCTION();
		glm::mat4& tMatrix = transform.GetTransformMat4();
		const Color& t = spriteRenderer.Color;
		glm::vec4 color = { t.r, t.g, t.b, t.a };

		SubmitQuad(tMatrix, color, spriteRenderer.Texture);
	}
	void HRenderer::SubmitQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		HZR_PROFILE_FUNCTION();
		SubmitQuad(transform, color, nullptr);	
	}
	void HRenderer::SubmitQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2DAsset> texture)
	{
		HZR_PROFILE_FUNCTION();
		DrawListStat& stat = s_Engine->GetDrawList().Stats;
		stat.QuadCount++;
		s_Engine->GetDrawList().Stats.Vertices++;

		s_Engine->GetQuadRenderer().SubmitQuad(transform, color, texture);
	}
	void HRenderer::SubmitMesh(const TransformComponent& transform, const MeshComponent& meshComponent)
	{
		HZR_PROFILE_FUNCTION();
		Ref<Mesh> mesh = meshComponent.m_MeshHandle;
		if (!mesh) return;
		if (!mesh->IsValid()) return;

		glm::mat4 t = transform.GetTransformMat4();

		SubmitMesh(t, mesh->GetVertexBuffer(), mesh->GetIndexBuffer(), mesh->GetPipeline());
		if (!meshComponent.CastShadows) return;

		SubmitShadowMesh(t, mesh->GetVertexBuffer(), mesh->GetIndexBuffer(), mesh->GetPipeline(), mesh->GetIndexCount());
	}
	void HRenderer::SubmitMesh(const glm::mat4& transform, Ref<VertexBuffer> vertexBuffer, Ref<Pipeline> pipeline, size_t count)
	{
		HZR_PROFILE_FUNCTION();
		SubmitMesh(transform, vertexBuffer, nullptr, pipeline, count);
	}
	void HRenderer::SubmitMesh(const glm::mat4& transform, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Ref<Pipeline> pipeline)
	{
		HZR_PROFILE_FUNCTION();
		SubmitMesh(transform, vertexBuffer, indexBuffer, pipeline, indexBuffer->GetCount());
	}
	void HRenderer::SubmitMesh(const glm::mat4& transform, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Ref<Pipeline> pipeline, size_t count)
	{
		HZR_PROFILE_FUNCTION();
		DrawListStat& stat = s_Engine->GetDrawList().Stats;
		stat.MeshCount++;
		stat.Vertices += (vertexBuffer->GetSize() / vertexBuffer->GetLayout().GetStride());
		stat.Indices += count;

		s_Engine->GetDrawList().MeshList[pipeline.Raw()].push_back({ transform, vertexBuffer, indexBuffer, count });
	}
	void HRenderer::SubmitShadowMesh(const glm::mat4& transform, Ref<VertexBuffer>& vertexBuffer, Ref<IndexBuffer>& indexBuffer, Ref<Pipeline>& pipeline, size_t count)
	{
		HZR_PROFILE_FUNCTION();
	}
	void HRenderer::SubmitPipeline(Ref<Pipeline>& pipeline, size_t count)
	{
		HZR_PROFILE_FUNCTION();
		//s_Engine->GetDrawList().Pipelines[pipeline.Raw()].push_back({ count });
	}
	void HRenderer::SubmitSkyLight(const SkyLightComponent& skyLight)
	{
		HZR_PROFILE_FUNCTION();
		if (skyLight.EnvironmentMap == nullptr) return;

		Ref<EnvironmentMap> map = skyLight.EnvironmentMap;
		EnvironmentData data = {};
		data.Color = skyLight.LightColor.ToGLM();
		data.IBLContribution = skyLight.Intensity;
		data.Map = map;

		s_Engine->GetDrawList().Environment[map.Raw()] = data;
	}
	void HRenderer::SubmitDirectionalLight(const TransformComponent& transform, DirectionalLightComponent& directionalLight)
	{
		HZR_PROFILE_FUNCTION();
		const glm::vec3 color = { directionalLight.LightColor.r, directionalLight.LightColor.g, directionalLight.LightColor.b };
		DirectionalLightSource source = {};
		source.Direction = Math::GetForwardDirection(transform.GetOrientation());
		source.Color = color;
		source.Intensity = directionalLight.Intensity;
		s_Engine->GetDrawList().DirectionalLights.push_back(source);
	}
	void HRenderer::SubmitPointLight(const TransformComponent& transform, PointLightComponent& pointLight)
	{
		HZR_PROFILE_FUNCTION();
		//s_Engine->GetDrawList().LightSource.push_back({ transform.GetTransformNoScale(), pointLight.LightColor, pointLight.Intensity });
	}

	void HRenderer::DrawPerspectiveCameraFrustum(const glm::vec3 position, const glm::quat& orientation, const glm::mat4& transform, float verticalFOV, glm::vec2 clipping, float aspectRatio, const Color& color)
	{
		HZR_PROFILE_FUNCTION();
		glm::vec4 c = { color.r, color.g, color.b, color.a };
		std::vector<glm::vec3> linePoints = Math::GetProjectionBounds(orientation, transform, verticalFOV, clipping.x, clipping.y, aspectRatio);

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Engine->GetLineRenderer().SubmitLine(linePoints[i] + position, linePoints[(i + 1) % 4] + position, c);
		}
		for (uint32_t i = 0; i < 4; i++)
		{
			s_Engine->GetLineRenderer().SubmitLine(linePoints[i] + position, linePoints[i + 4] + position, c);
		}
		for (uint32_t i = 0; i < 4; i++)
		{
			s_Engine->GetLineRenderer().SubmitLine(linePoints[i + 4] + position, linePoints[((i + 1) % 4) + 4] + position, c);
		}
	}
	void HRenderer::DrawOrthoCameraFrustum(const glm::vec3 position, const glm::quat& orientation, const glm::mat4& transform, float size, glm::vec2 clipping, float aspectRatio, const Color& color)
	{
		HZR_PROFILE_FUNCTION();
		glm::vec4 c = { color.r, color.g, color.b, color.a };
		std::vector<glm::vec3> linePoints = Math::GetProjectionBoundsOrtho(orientation, transform, size, clipping.x, clipping.y, aspectRatio);

		for (uint32_t i = 0; i < 4; i++)
		{
			s_Engine->GetLineRenderer().SubmitLine(linePoints[i] + position, linePoints[(i + 1) % 4] + position, c);
		}
		for (uint32_t i = 0; i < 4; i++)
		{
			s_Engine->GetLineRenderer().SubmitLine(linePoints[i] + position, linePoints[i + 4] + position, c);
		}
		for (uint32_t i = 0; i < 4; i++)
		{
			s_Engine->GetLineRenderer().SubmitLine(linePoints[i + 4] + position, linePoints[((i + 1) % 4) + 4] + position, c);
		}
	}
}
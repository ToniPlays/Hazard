
#include <hzrpch.h>
#include "HRenderer.h"
#include "Renderers/WorldRenderer.h"
#include "Hazard/Assets/AssetManager.h"

namespace Hazard
{
	using namespace HazardRenderer;

	void HRenderer::SubmitWorldRenderer(const Ref<WorldRenderer>& renderer)
	{
		HZR_PROFILE_FUNCTION();
		RendererDrawList list = {};
		list.WorldRenderer = renderer;
		s_Engine->GetDrawLists().push_back(list);
	}

	void HRenderer::SubmitSprite(TransformComponent& transform, const SpriteRendererComponent& spriteRenderer)
	{
		HZR_PROFILE_FUNCTION();
		const glm::mat4& tMatrix = transform.GetTransformMat4();
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
		stat.Vertices++;

		s_Engine->GetQuadRenderer().SubmitQuad(transform, color, texture);
	}
	void HRenderer::SubmitBillboard(const glm::mat4& transform, const glm::mat4& view, const glm::vec4& color, const Ref<Texture2DAsset>& texture)
	{
		s_Engine->GetQuadRenderer().SubmitBillboard(transform, view, color, texture);
	}
	void HRenderer::SubmitMesh(TransformComponent& transform, const MeshComponent& meshComponent)
	{
		HZR_PROFILE_FUNCTION();
		Ref<Mesh> mesh = meshComponent.m_MeshHandle;
		if (!mesh) return;
        
        Ref<Pipeline> pipeline = meshComponent.m_MaterialHandle ? meshComponent.m_MaterialHandle->GetPipeline() : s_Engine->GetResources().PbrPipeline;
        
		if (!mesh->IsValid()) return;

        auto& drawList = s_Engine->GetDrawList();
        
        const glm::mat4 t = transform.GetTransformMat4();
        
        MeshInstance instance = {};
        instance.Transform.MRow0 = { t[0][0], t[1][0], t[2][0], t[3][0] };
        instance.Transform.MRow1 = { t[0][1], t[1][1], t[2][1], t[3][1] };
        instance.Transform.MRow2 = { t[0][2], t[1][2], t[2][2], t[3][2] };
        
        auto& data = drawList.MeshList[pipeline.Raw()][mesh->GetVertexBuffer().Raw()];
        data.VertexBuffer = mesh->GetVertexBuffer();
        data.IndexBuffer = mesh->GetIndexBuffer();
        data.IndexCount = data.IndexBuffer->GetCount();
        
        auto& meshes = drawList.MeshList[pipeline.Raw()];
        meshes[data.VertexBuffer.Raw()].Instances.push_back(instance);

		//SubmitShadowMesh(data.Transform, data.VertexBuffer, data.IndexBuffer, pipeline, data.Count);
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
        
        auto& drawList = s_Engine->GetDrawList();
        
        const glm::mat4& t = transform;
        
        MeshInstance instance = {};
        instance.Transform.MRow0 = { t[0][0], t[1][0], t[2][0], t[3][0] };
        instance.Transform.MRow1 = { t[0][1], t[1][1], t[2][1], t[3][1] };
        instance.Transform.MRow2 = { t[0][2], t[1][2], t[2][2], t[3][2] };
        
        auto& data = drawList.MeshList[pipeline.Raw()][vertexBuffer.Raw()];
        data.VertexBuffer = vertexBuffer;
        data.IndexBuffer = indexBuffer;
        data.IndexCount = count;
        
        auto& meshes = drawList.MeshList[pipeline.Raw()];
        meshes[data.VertexBuffer.Raw()].Instances.push_back(instance);
	}

	void HRenderer::SubmitShadowMesh(const glm::mat4& transform, Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Ref<Pipeline> pipeline, size_t count)
	{
		HZR_PROFILE_FUNCTION();
	}
	void HRenderer::SubmitPipeline(Ref<Pipeline>& pipeline, size_t count)
	{
		HZR_PROFILE_FUNCTION();
		s_Engine->GetDrawList().Pipelines[pipeline.Raw()].push_back({ count });
	}
	void HRenderer::SubmitSkyLight(const SkyLightComponent& skyLight)
	{
		HZR_PROFILE_FUNCTION();
		if (skyLight.EnvironmentMap == nullptr) return;

		Ref<EnvironmentMap> map = skyLight.EnvironmentMap;
		EnvironmentData data = {};
		data.SkylightIntensity = skyLight.Intensity;
		data.EnvironmentLod = skyLight.LodLevel;
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

	void HRenderer::SubmitBoundingBox(const glm::mat4& transform, const BoundingBox& boundingBox, const Color& color)
	{
		HZR_PROFILE_FUNCTION();
		glm::vec4 c = { color.r, color.g, color.b, color.a };
		auto& lineRenderer = s_Engine->GetLineRenderer();

		glm::vec3 frontBottomLeft = transform * glm::vec4(boundingBox.MinX, boundingBox.MinY, boundingBox.MinZ, 1.0f);
		glm::vec3 frontBottomRight = transform * glm::vec4(boundingBox.MaxX, boundingBox.MinY, boundingBox.MinZ, 1.0f);

		glm::vec3 frontTopLeft = transform * glm::vec4(boundingBox.MinX, boundingBox.MaxY, boundingBox.MinZ, 1.0f);
		glm::vec3 frontTopRight = transform * glm::vec4(boundingBox.MaxX, boundingBox.MaxY, boundingBox.MinZ, 1.0f);

		glm::vec3 backBottomLeft = transform * glm::vec4(boundingBox.MinX, boundingBox.MinY, boundingBox.MaxZ, 1.0f);
		glm::vec3 backBottomRight = transform * glm::vec4(boundingBox.MaxX, boundingBox.MinY, boundingBox.MaxZ, 1.0f);

		glm::vec3 backTopLeft = transform * glm::vec4(boundingBox.MinX, boundingBox.MaxY, boundingBox.MaxZ, 1.0f);
		glm::vec3 backTopRight = transform * glm::vec4(boundingBox.MaxX, boundingBox.MaxY, boundingBox.MaxZ, 1.0f);

		//Draw bottom quad
		lineRenderer.SubmitLine(frontBottomLeft, frontBottomRight, c);
		lineRenderer.SubmitLine(frontBottomLeft, backBottomLeft, c);
		lineRenderer.SubmitLine(frontBottomRight, backBottomRight, c);
		lineRenderer.SubmitLine(backBottomLeft, backBottomRight, c);

		//Draw top quad
		lineRenderer.SubmitLine(frontTopLeft, frontTopRight, c);
		lineRenderer.SubmitLine(frontTopLeft, backTopLeft, c);
		lineRenderer.SubmitLine(frontTopRight, backTopRight, c);
		lineRenderer.SubmitLine(backTopLeft, backTopRight, c);

		//Draw the rest of it
		lineRenderer.SubmitLine(frontTopLeft, frontBottomLeft, c);
		lineRenderer.SubmitLine(frontTopRight, frontBottomRight, c);
		lineRenderer.SubmitLine(backTopLeft, backBottomLeft, c);
		lineRenderer.SubmitLine(backTopRight, backBottomRight, c);
	}

	void HRenderer::SubmitPerspectiveCameraFrustum(const glm::vec3 position1, const glm::quat& orientation, const glm::mat4& transform, float verticalFOV, glm::vec2 clipping, float aspectRatio, const Color& color)
	{
		HZR_PROFILE_FUNCTION();
		glm::vec4 c = { color.r, color.g, color.b, color.a };
		std::vector<glm::vec3> linePoints = Math::GetProjectionBounds(orientation, transform, verticalFOV, clipping.x, clipping.y, aspectRatio);

		const glm::vec3& position = transform[3];

		auto& lineRenderer = s_Engine->GetLineRenderer();

		for (uint32_t i = 0; i < 4; i++)
		{
			lineRenderer.SubmitLine(linePoints[i] + position, linePoints[(i + 1) % 4] + position, c);
		}
		for (uint32_t i = 0; i < 4; i++)
		{
			lineRenderer.SubmitLine(linePoints[i] + position, linePoints[i + 4] + position, c);
		}
		for (uint32_t i = 0; i < 4; i++)
		{
			lineRenderer.SubmitLine(linePoints[i + 4] + position, linePoints[((i + 1) % 4) + 4] + position, c);
		}
	}
	void HRenderer::SubmitOrthoCameraFrustum(const glm::vec3 position1, const glm::quat& orientation, const glm::mat4& transform, float size, glm::vec2 clipping, float aspectRatio, const Color& color)
	{
		HZR_PROFILE_FUNCTION();
		glm::vec4 c = { color.r, color.g, color.b, color.a };
		std::vector<glm::vec3> linePoints = Math::GetProjectionBoundsOrtho(orientation, transform, size, clipping.x, clipping.y, aspectRatio);

		const glm::vec3& position = transform[3];

		auto& lineRenderer = s_Engine->GetLineRenderer();

		for (uint32_t i = 0; i < 4; i++)
		{
			lineRenderer.SubmitLine(linePoints[i] + position, linePoints[(i + 1) % 4] + position, c);
		}
		for (uint32_t i = 0; i < 4; i++)
		{
			lineRenderer.SubmitLine(linePoints[i] + position, linePoints[i + 4] + position, c);
		}
		for (uint32_t i = 0; i < 4; i++)
		{
			lineRenderer.SubmitLine(linePoints[i + 4] + position, linePoints[((i + 1) % 4) + 4] + position, c);
		}
	}
}

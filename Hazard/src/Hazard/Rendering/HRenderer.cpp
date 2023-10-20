
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

	void HRenderer::SubmitSprite(TransformComponent& transform, const SpriteRendererComponent& spriteRenderer, int id)
	{
		HZR_PROFILE_FUNCTION();
		const glm::mat4& tMatrix = transform.GetTransformMat4();
		const Color& t = spriteRenderer.Color;
		glm::vec4 color = { t.r, t.g, t.b, t.a };

		SubmitQuad(tMatrix, color, AssetManager::GetAsset<Texture2DAsset>(spriteRenderer.TextureHandle), id);
	}
	void HRenderer::SubmitQuad(const glm::mat4& transform, const glm::vec4& color, int id)
	{
		HZR_PROFILE_FUNCTION();
		SubmitQuad(transform, color, nullptr, id);
	}
	void HRenderer::SubmitQuad(const glm::mat4& transform, const glm::vec4& color, const Ref<Texture2DAsset> texture, int id)
	{
		HZR_PROFILE_FUNCTION();
		DrawListStat& stat = s_Engine->GetDrawList().Stats;
		stat.QuadCount++;
		stat.Vertices++;

		s_Engine->GetQuadRenderer().SubmitQuad(transform, color, texture);
	}
	void HRenderer::SubmitCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade, int id)
	{
		HZR_PROFILE_FUNCTION();
		DrawListStat& stat = s_Engine->GetDrawList().Stats;
		stat.QuadCount++;
		stat.Vertices++;

		s_Engine->GetCircleRenderer().SubmitCircle(transform, color, thickness, fade);
	}
	void HRenderer::SubmitBillboard(const glm::mat4& transform, const glm::mat4& view, const glm::vec4& color, const Ref<Texture2DAsset>& texture)
	{
		s_Engine->GetQuadRenderer().SubmitBillboard(transform, view, color, texture);
	}
	void HRenderer::SubmitMesh(TransformComponent& transform, const MeshComponent& meshComponent, int id)
	{
		HZR_PROFILE_FUNCTION();
		HZR_TIMED_FUNCTION();

		Ref<Mesh> mesh = AssetManager::GetAsset<Mesh>(meshComponent.MeshHandle);
		if (!mesh) return;
		SubmitMesh(transform.GetTransformMat4(), mesh->GetVertexBuffer(), mesh->GetIndexBuffer(), mesh->GetMaterial(), id);
	}
	void HRenderer::SubmitMesh(const glm::mat4& transform, Ref<GPUBuffer> vertexBuffer, Ref<Material> material, uint64_t count, int id)
	{
		HZR_PROFILE_FUNCTION();
		SubmitMesh(transform, vertexBuffer, nullptr, material, count, id);
	}
	void HRenderer::SubmitMesh(const glm::mat4& transform, Ref<GPUBuffer> vertexBuffer, Ref<GPUBuffer> indexBuffer, Ref<Material> material, int id)
	{
		HZR_PROFILE_FUNCTION();
		SubmitMesh(transform, vertexBuffer, indexBuffer, material, indexBuffer->GetSize() / sizeof(uint32_t), id);
	}
	void HRenderer::SubmitMesh(const glm::mat4& transform, Ref<GPUBuffer> vertexBuffer, Ref<GPUBuffer> indexBuffer, Ref<Material> material, uint64_t count, int id)
	{
		HZR_PROFILE_FUNCTION();
		HZR_TIMED_FUNCTION();

		if (count == 0) return;

		Ref<Pipeline> pipeline = AssetManager::GetAsset<AssetPointer>(material->GetPipeline())->Value.As<Pipeline>();

		auto& drawList = s_Engine->GetDrawList();
		{
			GraphInstruction& instruction = drawList.GeometryPassInstructions.emplace_back();
			uint64_t index;
			if (drawList.Buffers.Contains(vertexBuffer.Raw()))
				index = drawList.Buffers.GetIndex(vertexBuffer.Raw());
			else index = drawList.Buffers.Push(vertexBuffer.Raw(), { .VertexBuffer = vertexBuffer.Raw() });

			instruction.SetVertexBuffer(1, index, 0);
		}
		{
			GraphInstruction& instruction = drawList.GeometryPassInstructions.emplace_back();
			uint64_t index;
			if (drawList.Pipelines.Contains(pipeline.Raw()))
				index = drawList.Pipelines.GetIndex(pipeline.Raw());
			else 
				index = drawList.Pipelines.Push(pipeline.Raw(), { .Pipeline = pipeline.Raw() });
			instruction.SetPipeline(2, index);
		}
		{
			drawList.PushConstantData.push_back(Buffer((void*)&transform, sizeof(glm::mat4)));
			uint32_t index = drawList.PushConstantData.size() - 1;
			drawList.PushConstantBuffers.push_back(ResourceReference{ .PushConstantBuffer = &drawList.PushConstantData[index] });

			GraphInstruction& instruction = drawList.GeometryPassInstructions.emplace_back();
			instruction.PushConstants(3, drawList.PushConstantData.size() - 1, sizeof(glm::mat4));
		}
		{
			if (material->GetDescriptorSet())
			{
				DescriptorSet* set = material->GetDescriptorSet().Raw();
				GraphInstruction& instruction = drawList.GeometryPassInstructions.emplace_back();
				instruction.SetDescriptor(1, drawList.Buffers.Push(set, { .DescriptorSet = set }), 1);
			}
		}
		{
			if (indexBuffer)
			{
				GraphInstruction& instruction = drawList.GeometryPassInstructions.emplace_back();

				uint64_t index;
				if (drawList.Buffers.Contains(indexBuffer.Raw()))
					index = drawList.Buffers.GetIndex(indexBuffer.Raw());
				else index = drawList.Buffers.Push(indexBuffer.Raw(), { .IndexBuffer = indexBuffer.Raw() });

				instruction.Draw(count, 1, index);
			}
			else
			{
				GraphInstruction& instruction = drawList.GeometryPassInstructions.emplace_back();
				instruction.Draw(count);
			}
		}
	}

	void HRenderer::SubmitShadowMesh(const glm::mat4& transform, Ref<GPUBuffer> vertexBuffer, Ref<GPUBuffer> indexBuffer, Ref<Material> material, uint64_t count)
	{
		HZR_PROFILE_FUNCTION();
	}
	void HRenderer::SubmitPipeline(Ref<Pipeline>& pipeline, uint64_t count)
	{
		HZR_PROFILE_FUNCTION();
	}
	void HRenderer::SubmitSkyLight(const SkyLightComponent& skyLight)
	{
		HZR_PROFILE_FUNCTION();
		if (skyLight.EnvironmentMapHandle == INVALID_ASSET_HANDLE) return;

		Ref<EnvironmentMap> map = AssetManager::GetAsset<EnvironmentMap>(skyLight.EnvironmentMapHandle);
		if (!map) return;
		if (!map->RadianceMap) return;

		AssetHandle skyboxHandle = s_Engine->GetResources().SkyboxMaterialHandle;
		Ref<Material> skyboxMaterial = AssetManager::GetAsset<Material>(skyboxHandle);

		skyboxMaterial->GetDescriptorSet()->Write(0, 0, map->RadianceMap->Value.As<Image2D>(), s_Engine->GetResources().DefaultImageSampler);
		Ref<Pipeline> pipeline = AssetManager::GetAsset<AssetPointer>(skyboxMaterial->GetPipeline())->Value.As<Pipeline>();

		auto& drawList = s_Engine->GetDrawList();
		{
			GraphInstruction& instruction = drawList.SkyboxInstructions.emplace_back();
			instruction.SetPipeline(2, drawList.Pipelines.Push(pipeline.Raw(), { .Pipeline = pipeline.Raw() }));
		}
		{
			/*
			float values[] = { 0.25f, 1.0f };

			drawList.PushConstantData.push_back(Buffer((void*)values, sizeof(float) * 2));
			uint32_t index = drawList.PushConstantData.size() - 1;
			drawList.PushConstantBuffers.push_back(ResourceReference{ .PushConstantBuffer = &drawList.PushConstantData[index] });

			GraphInstruction& instruction = drawList.SkyboxInstructions.emplace_back();
			instruction.PushConstants(3, drawList.PushConstantData.size() - 1, sizeof(float) * 2);
			*/
		}
		{
			DescriptorSet* set = skyboxMaterial->GetDescriptorSet().Raw();
			GraphInstruction& instruction = drawList.SkyboxInstructions.emplace_back();
			instruction.SetDescriptor(1, drawList.Buffers.Push(set, { .DescriptorSet = set }), 1);
		}
		{
			GraphInstruction& instruction = drawList.SkyboxInstructions.emplace_back();
			instruction.Draw(6);
		}
	}
	void HRenderer::SubmitDirectionalLight(const TransformComponent& transform, DirectionalLightComponent& directionalLight)
	{
		HZR_PROFILE_FUNCTION();
	}
	void HRenderer::SubmitPointLight(const TransformComponent& transform, PointLightComponent& pointLight)
	{
		HZR_PROFILE_FUNCTION();
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
			lineRenderer.SubmitLine(linePoints[i] + position, linePoints[(i + 1) % 4] + position, c);
		for (uint32_t i = 0; i < 4; i++)
			lineRenderer.SubmitLine(linePoints[i] + position, linePoints[i + 4] + position, c);
		for (uint32_t i = 0; i < 4; i++)
			lineRenderer.SubmitLine(linePoints[i + 4] + position, linePoints[((i + 1) % 4) + 4] + position, c);
	}
}


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
	void HRenderer::SubmitMesh(const glm::mat4& transform, Ref<GPUBuffer> vertexBuffer, Ref<Material> material, size_t count, int id)
	{
		HZR_PROFILE_FUNCTION();
		SubmitMesh(transform, vertexBuffer, nullptr, material, count, id);
	}
	void HRenderer::SubmitMesh(const glm::mat4& transform, Ref<GPUBuffer> vertexBuffer, Ref<GPUBuffer> indexBuffer, Ref<Material> material, int id)
	{
		HZR_PROFILE_FUNCTION();
		SubmitMesh(transform, vertexBuffer, indexBuffer, material, indexBuffer->GetSize() / sizeof(uint32_t), id);
	}
	void HRenderer::SubmitMesh(const glm::mat4& transform, Ref<GPUBuffer> vertexBuffer, Ref<GPUBuffer> indexBuffer, Ref<Material> material, size_t count, int id)
	{
		HZR_PROFILE_FUNCTION();
		HZR_TIMED_FUNCTION();

		if (count == 0) return;

		Ref<Pipeline> pipeline = AssetManager::GetAsset<AssetPointer>(material->GetPipeline())->Value.As<Pipeline>();

		auto& drawList = s_Engine->GetDrawList();
		{
			drawList.Buffers.push_back({ .VertexBuffer = vertexBuffer.Raw() });

			GraphInstruction& instruction = drawList.GeometryPassInstructions.emplace_back();
			instruction.Flags = INSTRUCTION_BIND_VERTEX_BUFFER;
			instruction.DataSource = 1;
			instruction.Source.VertexBufferIndex = drawList.Buffers.size() - 1;
			instruction.Destination.BindingIndex = 0;
		}
		{
			drawList.Buffers.push_back({ .Pipeline = pipeline.Raw() });

			GraphInstruction& instruction = drawList.GeometryPassInstructions.emplace_back();
			instruction.Flags = INSTRUCTION_BIND_PIPELINE;
			instruction.DataSource = 1;
			instruction.Source.PipelineIndex = drawList.Buffers.size() - 1;
		}
		{
			drawList.PushConstantData.push_back(Buffer::Copy(glm::value_ptr(transform), sizeof(glm::mat4)));
			uint32_t index = drawList.PushConstantData.size() - 1;

			drawList.PushConstantBuffers.push_back(ResourceReference{ .PushConstantBuffer = &drawList.PushConstantData[index] });

			GraphInstruction& instruction = drawList.GeometryPassInstructions.emplace_back();
			instruction.Flags = INSTRUCTION_PUSH_CONSTANTS;
			instruction.DataSource = 2;
			instruction.Source.PushConstantIndex = drawList.PushConstantData.size() - 1;
			instruction.DataSize = sizeof(glm::mat4);
		}
		{
			drawList.Buffers.push_back({ .DescriptorSet = material->GetDescriptorSet().Raw() });

			GraphInstruction& instruction = drawList.GeometryPassInstructions.emplace_back();
			instruction.Flags = INSTRUCTION_BIND_DESCRIPTOR_SET;
			instruction.DataSource = 1;
			instruction.Source.DescriptorSetIndex = drawList.Buffers.size() - 1;
			instruction.Destination.BindingIndex = 0;
		}
		{
			GraphInstruction& instruction = drawList.GeometryPassInstructions.emplace_back();
			instruction.Flags = INSTRUCTION_DRAW;
			instruction.DataSource = 1;
			instruction.Source.IndexBufferIndex = drawList.Buffers.size();
			instruction.Destination.DrawCount = count;

			if (indexBuffer)
				drawList.Buffers.push_back({ .IndexBuffer = indexBuffer.Raw() });
			else
				instruction.Flags |= INSTRUCTION_NO_SOURCE;
		}
	}

	void HRenderer::SubmitShadowMesh(const glm::mat4& transform, Ref<GPUBuffer> vertexBuffer, Ref<GPUBuffer> indexBuffer, Ref<Material> material, size_t count)
	{
		HZR_PROFILE_FUNCTION();
	}
	void HRenderer::SubmitPipeline(Ref<Pipeline>& pipeline, size_t count)
	{
		HZR_PROFILE_FUNCTION();
	}
	void HRenderer::SubmitSkyLight(const SkyLightComponent& skyLight)
	{
		HZR_PROFILE_FUNCTION();
		if (skyLight.EnvironmentMap == nullptr) return;

		Ref<EnvironmentMap> map = skyLight.EnvironmentMap;
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

#include "hzrpch.h"
#include "RasterizedRenderer.h"
#include "Hazard/Rendering/RenderResources.h"
#include "Hazard/Core/Application.h"
#include "Hazard/Rendering/RenderEngine.h"

namespace Hazard 
{
	using namespace HazardRenderer;

	void RasterizedRenderer::Prepare(Ref<HazardRenderer::RenderPass> renderPass, const RendererDrawList& drawList)
	{
		m_RenderPass = renderPass;

		auto& resources = Application::GetModule<RenderEngine>().GetResources();
		LightingData data = {};
		data.DirectionLightCount = drawList.DirectionalLights.size();

		for (uint32_t i = 0; i < data.DirectionLightCount; i++)
		{
			auto& light = drawList.DirectionalLights[i];
			data.Lights[i].Direction = glm::vec4(glm::normalize(light.Direction), 1.0);
			data.Lights[i].Color = glm::vec4(light.Color, light.Intensity);
		}

		if (drawList.Environment.size() > 0)
		{
			for (auto& [map, environmentData] : drawList.Environment)
			{
				data.SkyLightIntensity = environmentData.SkylightIntensity;
				data.EnvironmentLod = environmentData.EnvironmentLod;

				auto& radiance = environmentData.Map->RadianceMap;
				auto& irradiance = environmentData.Map->IrradianceMap;
				auto& prefilter = environmentData.Map->PreFilterMap;
				auto& lut = environmentData.Map->BRDFLut;

				auto& shader = resources.PbrPipeline->GetShader();
				if (radiance)
					shader->Set("u_RadianceMap", 0, radiance->Value ? radiance->Value.As<CubemapTexture>() : resources.WhiteCubemap);
				if (irradiance)
					shader->Set("u_IrradianceMap", 0, irradiance->Value ? irradiance->Value.As<CubemapTexture>() : resources.WhiteCubemap);
				if (prefilter)
					shader->Set("u_PrefilterMap", 0, prefilter->Value ? prefilter->Value.As<CubemapTexture>() : resources.WhiteCubemap);
				if (lut)
					shader->Set("u_BRDFLut", 0, lut->Value.As<Image2D>());
				break;
			}
		}
		else
		{
			data.SkyLightIntensity = 0.0f;
			data.EnvironmentLod = 0.0f;

			auto& shader = resources.PbrPipeline->GetShader();
			shader->Set("u_RadianceMap", 0, resources.WhiteCubemap);
			shader->Set("u_IrradianceMap", 0, resources.WhiteCubemap);
			shader->Set("u_PrefilterMap", 0, resources.WhiteCubemap);
			shader->Set("u_BRDFLut", 0, resources.BRDFLut->GetSourceImageAsset()->Value.As<Image2D>());
		}

		//Update buffers
		m_CommandBuffer->BindUniformBuffer(resources.LightUniformBuffer);

		BufferCopyRegion region = {};
		region.Data = &data;
		region.Size = sizeof(LightingData);

		resources.LightUniformBuffer->SetData(region);
	}

	void RasterizedRenderer::GeometryPass(const std::unordered_map<Pipeline*, std::vector<RawMesh>>& meshList)
	{
		auto& modelUniformBuffer = Application::GetModule<RenderEngine>().GetResources().ModelUniformBuffer;
		for (auto& [pipeline, meshList] : meshList)
		{
			pipeline->SetRenderPass(m_RenderPass);

			if (!pipeline->IsValid()) continue;

			m_CommandBuffer->BindPipeline(pipeline);

			for (auto& mesh : meshList)
			{
				ModelData data = {};
				data.Transform = mesh.Transform;
				data.Metalness = mesh.Metalness;
				data.Roughness = mesh.Roughness;
				data.Flags = 0;

				BufferCopyRegion region = {};
				region.Data = &data;
				region.Size = sizeof(ModelData);

				modelUniformBuffer->SetData(region);
				m_CommandBuffer->BindVertexBuffer(mesh.VertexBuffer);
				m_CommandBuffer->Draw(mesh.Count, mesh.IndexBuffer);
			}
		}
	}
	void RasterizedRenderer::EnvironmentPass(const std::unordered_map<EnvironmentMap*, EnvironmentData>& environment)
	{
		HZR_PROFILE_FUNCTION();
		for (auto& [map, environmentData] : environment)
		{
			auto& radiance = environmentData.Map->RadianceMap;
			if (!radiance) return;
			auto& resources = Application::GetModule<RenderEngine>().GetResources();

			resources.SkyboxPipeline->GetShader()->Set("u_CubeMap", 0, radiance->Value.As<CubemapTexture>());
			m_CommandBuffer->BindPipeline(resources.SkyboxPipeline);
			m_CommandBuffer->Draw(6);
			return;
		}
	}
	void RasterizedRenderer::CompositePass(const std::unordered_map<Pipeline*, std::vector<PipelineData>>& list)
	{
		HZR_PROFILE_FUNCTION();
		for (auto& [pipeline, usages] : list)
		{
			pipeline->SetRenderPass(m_RenderPass);

			if (!pipeline->IsValid()) continue;
			m_CommandBuffer->BindPipeline(pipeline);

			for (auto& data : usages)
				m_CommandBuffer->Draw(data.Count);
		}
	}
}
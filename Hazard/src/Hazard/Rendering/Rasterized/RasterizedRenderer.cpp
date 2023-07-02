#include "hzrpch.h"
#include "RasterizedRenderer.h"
#include "Hazard/Rendering/RenderResources.h"
#include "Hazard/Core/Application.h"
#include "Hazard/Rendering/RenderEngine.h"

namespace Hazard
{
	using namespace HazardRenderer;

	void RasterizedRenderer::Render(Ref<HazardRenderer::RenderCommandBuffer> buffer, const RendererDrawList& drawList)
	{
		m_Buffer = buffer;
		m_DrawCalls.clear();
		PreRender(drawList);

		for (auto& camera : drawList.WorldRenderer->GetCameraData())
		{
			PrepareCamera(camera);
			m_CurrentRenderPass = camera.RenderPass;
			m_Buffer->BeginRenderPass(camera.RenderPass);
			CompositePass(drawList.Environment);
			m_Buffer->EndRenderPass();
		}
	}

	void RasterizedRenderer::PrepareCamera(const WorldCameraData& camera)
	{
		auto& resources = RenderEngine::GetResources();
		//Setup camera uniform

		CameraData data = {};
		data.ViewProjection = camera.Projection * camera.View;
		data.Projection = camera.Projection;
		data.InverseProjection = glm::inverse(camera.Projection);
		data.View = camera.View;
		data.InverseView = glm::inverse(camera.View);
		data.InverseViewProjection = data.InverseView * data.InverseViewProjection;

		BufferCopyRegion region = {};
		region.Data = &data;
		region.Size = sizeof(CameraData);

		//resources.CameraUniformBuffer->SetData(region);
		//Setup utility buffer

		UtilityUniformData utils = {};
		utils.CameraPos = data.InverseView[3];
		utils.Flags = 0;

		region.Data = &utils;
		region.Size = sizeof(UtilityUniformData);

		//resources.UtilityUniformBuffer->SetData(region);
		std::vector<Ref<GPUBuffer>> buffers = { resources.CameraUniformBuffer, resources.UtilityUniformBuffer };
		//m_Buffer->SetUniformBuffers(buffers.data(), 2);

	}

	void RasterizedRenderer::PreRender(const RendererDrawList& drawList)
	{
		auto& resources = RenderEngine::GetResources();
		//Initialize lighting data
		{
			LightingData data = {};
			data.SkyLightIntensity = drawList.Environment.SkylightIntensity;
			data.EnvironmentLod = drawList.Environment.EnvironmentLod;

			BufferCopyRegion region = {};
			region.Data = &data;
			region.Size = sizeof(LightingData);

			//resources.LightUniformBuffer->SetData(region);
		}
		//m_Buffer->SetUniformBuffers(&resources.LightUniformBuffer, 1);

		//Initialize mesh transform data

		size_t offset = 0;

		for (auto& [material, meshes] : drawList.MeshList)
		{
			if (!material) continue;

			for (auto& [buffer, mesh] : meshes)
			{
				DrawCall& call = m_DrawCalls.emplace_back();
				call.Material = material;
				call.VertexBuffer = mesh.VertexBuffer;
				call.IndexBuffer = mesh.IndexBuffer;
				call.InstanceCount = mesh.Instances.size();
				call.IndexCount = mesh.IndexCount;
				call.TransformOffset = offset;

				BufferCopyRegion region = {};
				region.Data = mesh.Instances.data();
				region.Size = mesh.Instances.size() * sizeof(MeshInstance);
				region.Offset = offset;

				//resources.TransformBuffer->SetData(region);

				offset += region.Size;
			}
		}
	}
	void RasterizedRenderer::CompositePass(const EnvironmentData& environment)
	{
		auto& resources = RenderEngine::GetResources();

		//Prepare Materials
		{
			/*
			auto shader = resources.PBRMaterial->GetPipeline()->GetShader();
			if (!environment.Map)
			{
				shader->Set("u_RadianceMap", 0, resources.WhiteCubemap);
				shader->Set("u_IrradianceMap", 0, resources.WhiteCubemap);
				shader->Set("u_PrefilterMap", 0, resources.WhiteCubemap);
				shader->Set("u_BRDFLut", 0, resources.BRDFLut->GetSourceImageAsset()->Value.As<Image2D>());
			}
			else
			{
				auto& radiance = environment.Map->RadianceMap;
				auto& irradiance = environment.Map->IrradianceMap;
				auto& prefilter = environment.Map->PreFilterMap;
				auto& lut = environment.Map->BRDFLut;

				if (radiance)
					shader->Set("u_RadianceMap", 0, radiance->Value ? radiance->Value.As<CubemapTexture>() : resources.WhiteCubemap);
				if (irradiance)
					shader->Set("u_IrradianceMap", 0, irradiance->Value ? irradiance->Value.As<CubemapTexture>() : resources.WhiteCubemap);
				if (prefilter)
					shader->Set("u_PrefilterMap", 0, prefilter->Value ? prefilter->Value.As<CubemapTexture>() : resources.WhiteCubemap);
				if (lut)
					shader->Set("u_BRDFLut", 0, lut->GetSourceImageAsset()->Value.As<Image2D>());
			}
			*/
		}
		//Draw all meshes
		{
			for (DrawCall& call : m_DrawCalls)
			{
				auto pipeline = AssetManager::GetAsset<AssetPointer>(call.Material->GetPipeline())->Value.As<Pipeline>();
				auto shader = pipeline->GetShader();

				//Bind textures
				for (auto& [name, texture] : call.Material->GetTextures())
				{
					Ref<Texture2DAsset> asset = AssetManager::GetAsset<Texture2DAsset>(texture);
					//if (texture)
						//shader->Set(name, 0, asset->GetSourceImageAsset()->Value.As<Image2D>());
				}

				//shader->Set("u_MaterialConstants", call.Material->GetBuffer());
				//shader->Set(PerInstance, resources.TransformBuffer, call.TransformOffset);

				m_Buffer->SetPipeline(pipeline);
				m_Buffer->SetVertexBuffer(call.VertexBuffer);
				m_Buffer->DrawInstanced(call.IndexCount, call.InstanceCount, call.IndexBuffer);
			}
		}
		if (!environment.Map) return;

		auto& radiance = environment.Map->RadianceMap;

		if (!radiance) return;

		auto skyboxMaterial = AssetManager::GetAsset<AssetPointer>(RenderEngine::GetResources().SkyboxPipelineHandle);
		auto materialHandle = ShaderLibrary::GetPipelineAssetHandle("Skybox");
		auto material = AssetManager::GetAsset<AssetPointer>(materialHandle)->Value.As<Pipeline>();

		material->SetRenderPass(m_CurrentRenderPass);
		//material->GetShader()->Set("u_CubeMap", 0, radiance->Value.As<CubemapTexture>());
		m_Buffer->SetPipeline(material);
		m_Buffer->Draw(6);
	}
}

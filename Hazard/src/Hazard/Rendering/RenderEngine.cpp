#pragma once

#include <hzrpch.h>
#include "RenderEngine.h"
#include "RenderCommand.h"
#include "Loaders/Loaders.h"
#include "Hazard/Assets/AssetManager.h"
#include "2D/Renderer2D.h"
#include "Mesh/MeshFactory.h"

namespace Hazard::Rendering
{
	RenderEngine::RenderEngine(RenderEngineCreateInfo* info, RenderAPI api) : Module("RenderEngine")
	{
		HZR_PROFILE_FUNCTION();
		SetActive(true);

		RenderCommand::s_Engine = this;
		RenderCommand::s_Api = api;

		AssetManager::RegisterLoader<TextureLoader>(AssetType::Image);
		AssetManager::RegisterLoader<MeshLoader>(AssetType::Mesh);

		m_RenderCommandBuffer = RenderCommandBuffer::Create("RenderEngine");
		AssetHandle handle = AssetManager::ImportAsset("C:/dev/HazardProject/assets/Models/c8_Corvette_colored.fbx");
		m_TestMesh = AssetManager::GetAsset<Mesh>(handle);

		WindowResizeEvent e = { 1920, 1080 };
		OnResize(e);

		m_RenderPass = m_TestMesh->GetPipeline()->GetSpecifications().RenderPass;
		m_Renderer2D = new Renderer2D(info, m_RenderCommandBuffer);
		m_Renderer2D->Recreate(m_RenderPass);
	}
	RenderEngine::~RenderEngine()
	{
		delete m_Renderer2D;
	}
	void RenderEngine::Render()
	{
		m_RenderCommandBuffer->Begin();
		m_RenderPassData.ViewProjection = m_RenderingCamera->GetViewPprojection();

		struct Model {
			glm::mat4 transform { 1.0f };
		} model;

		model.transform = Math::ToTransformMatrix(glm::vec3(0.0f), { glm::radians(0.0), glm::radians(Time::s_Time * 5.0f), glm::radians(0.0) });
		
		Ref<Pipeline> meshPipeline = m_TestMesh->GetPipeline();
		RenderCommand::BeginRenderPass(m_RenderCommandBuffer, meshPipeline->GetSpecifications().RenderPass);

		m_Renderer2D->Render(m_RenderPassData);
		meshPipeline->GetShader()->SetUniformBuffer("Camera", &m_RenderPassData);
		meshPipeline->GetShader()->SetUniformBuffer("Model", &model);

		meshPipeline->GetSpecifications().RenderPass;
		meshPipeline->Bind(m_RenderCommandBuffer);
		meshPipeline->Draw(m_RenderCommandBuffer, m_TestMesh->GetIndexCount());

		RenderCommand::EndRenderPass(m_RenderCommandBuffer);
		m_RenderCommandBuffer->End();
		m_RenderCommandBuffer->Submit();
	}
	bool RenderEngine::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		return dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT(RenderEngine::OnResize));
	}
	void RenderEngine::Close()
	{
		HZR_PROFILE_FUNCTION();
	}
	bool RenderEngine::OnResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0 && !m_RenderPass->GetSpecs().TargetFrameBuffer->GetSpecification().SwapChainTarget) return false;

		SetViewportSize(e.GetWidth(), e.GetHeight());

		return true;
	}
	void RenderEngine::SetViewportSize(uint32_t width, uint32_t height) 
	{
		constexpr float size = 2.0f;
		float aspectRatio = (float)width / (float)height;
		float scalar = aspectRatio * size;

		//m_ViewProjection = glm::ortho(-scalar, scalar, -size, size, -1000.0f, 1000.0f);
		m_Projection = glm::perspective(glm::radians(60.0f), aspectRatio, 0.003f, 1000.0f);
	}
}

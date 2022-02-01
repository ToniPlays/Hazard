
#include <hzrpch.h>
#include "RenderEngine.h"
#include "RenderCommand.h"
#include "Loaders/Loaders.h"
#include "Hazard/Assets/AssetManager.h"
#include "2D/Renderer2D.h"
#include "DebugRenderer.h"

namespace Hazard::Rendering
{
	RenderEngine::RenderEngine(RenderEngineCreateInfo* info) : Module("RenderEngine")
	{
		SetActive(info->Enabled);

		RenderCommand::s_Engine = this;

		AssetManager::RegisterLoader<TextureLoader>(AssetType::Image);
		AssetManager::RegisterLoader<MeshLoader>(AssetType::Mesh);

		m_RenderCommandBuffer = RenderCommandBuffer::Create("RenderEngine");

		m_Queue = new CommandQueue();
		m_PostPassQueue = new CommandQueue(1024 * 1024);

		WindowResizeEvent e = { 1920, 1080 };
		OnResize(e);

		uint32_t data = 0xFFFFFFFF;
		Texture2DCreateInfo whiteTextureInfo = {};
		whiteTextureInfo.Width = 1;
		whiteTextureInfo.Height = 1;
		whiteTextureInfo.Data = &data;
		whiteTextureInfo.Usage = ImageUsage::Texture;
		whiteTextureInfo.Format = ImageFormat::RGBA;

		m_WhiteTexture = Texture2D::Create(&whiteTextureInfo);
		m_Renderer2D = new Renderer2D(info, m_RenderCommandBuffer);
		m_DebugRenderer = new DebugRenderer(1500, m_RenderCommandBuffer);

		UniformBufferCreateInfo uboInfo = {};
		uboInfo.Name = "Camera";
		uboInfo.Usage = BufferUsage::DynamicDraw;
		uboInfo.Size = sizeof(RenderPassData) * 4;
		uboInfo.Binding = 0;

		m_CameraUBO = UniformBuffer::Create(&uboInfo);
	}
	RenderEngine::~RenderEngine()
	{

	}
	void RenderEngine::Render()
	{
		HZR_PROFILE_FUNCTION();
		for (auto& worldRenderer : WorldRenderer::s_Renderers)
		{
			worldRenderer->Invalidate();
		}

		m_RenderCommandBuffer->Begin();
		m_CameraUBO->Bind(m_RenderCommandBuffer);

		for (auto& worldRenderer : WorldRenderer::s_Renderers)
		{
			if (!worldRenderer->IsValid()) continue;
			ProcessWorldRenderer(worldRenderer.Raw());
		}

		m_RenderCommandBuffer->End();
		m_RenderCommandBuffer->Submit();

		m_Queue->Clear();
		m_PostPassQueue->Clear();
	}
	bool RenderEngine::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		return dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT(RenderEngine::OnResize));
	}
	void RenderEngine::Close()
	{
		delete m_Renderer2D;
		delete m_DebugRenderer;

		HZR_CORE_ASSERT(m_Queue->Empty(), "Queue not empty");
		HZR_CORE_ASSERT(m_PostPassQueue->Empty(), "Queue not empty");

		m_WhiteTexture.Reset();
		m_CameraUBO.Reset();

		WorldRenderer::s_Renderers.clear();
	}
	bool RenderEngine::OnResize(WindowResizeEvent& e)
	{
		return false;
	}
	void RenderEngine::ProcessWorldRenderer(WorldRenderer* renderer)
	{
		HZR_PROFILE_FUNCTION();
		renderer->Begin(m_RenderCommandBuffer, m_Queue);

		if (renderer->m_Settings.Flags & WorldRenderFlags_::Enabled)
		{
			Camera* cam = renderer->GetSettings().Camera;

			m_RenderPassData.ViewProjection = cam->GetViewPprojection();
			m_RenderPassData.Projection = cam->GetProjection();
			m_RenderPassData.View = cam->GetView();
			m_RenderPassData.Position = glm::vec4(cam->GetPosition(), 1.0);
			m_CameraUBO->SetData(&m_RenderPassData);
			m_CurrentRenderPass = renderer->GetRenderPass();


			if (renderer->m_Settings.Flags & WorldRenderFlags_::Geometry)
			{
				m_Renderer2D->SetTargetRenderPass(m_CurrentRenderPass);
				m_DebugRenderer->SetTargetRenderPass(m_CurrentRenderPass);

				m_Renderer2D->BeginWorld((WorldRenderFlags_)renderer->m_Settings.Flags);
				m_DebugRenderer->BeginWorld((WorldRenderFlags_)renderer->m_Settings.Flags);

				m_Queue->Excecute();

				m_Renderer2D->EndWorld();
				m_DebugRenderer->EndWorld();

				m_PostPassQueue->Excecute();
			}
		}
		renderer->End(m_RenderCommandBuffer);
	}
}

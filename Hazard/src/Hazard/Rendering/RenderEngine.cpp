
#include <hzrpch.h>
#include "RenderEngine.h"
#include "RenderCommand.h"
#include "Loaders/Loaders.h"
#include "Hazard/Assets/AssetManager.h"
#include "2D/Renderer2D.h"
#include "DebugRenderer.h"
#include "WorldRenderer.h"

namespace Hazard::Rendering
{
	RenderEngine::RenderEngine(RenderEngineCreateInfo* info) : Module("RenderEngine")
	{
		HZR_PROFILE_FUNCTION();
		SetActive(info->Enabled);

		RenderCommand::s_Engine = this;
		RenderCommand::s_Api = Application::GetModule<RenderContext>().GetCurrentAPI();

        HZR_CORE_INFO("What do you mean dude?");
        
		AssetManager::RegisterLoader<TextureLoader>(AssetType::Image);
		AssetManager::RegisterLoader<MeshLoader>(AssetType::Mesh);
       
		m_RenderCommandBuffer = RenderCommandBuffer::Create("RenderEngine");
		m_Queue = new RenderCommandQueue();
		m_PostPassQueue = new RenderCommandQueue(1024 * 1024);

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
	}
	RenderEngine::~RenderEngine()
	{
		delete m_Renderer2D;
		delete m_DebugRenderer;
	}
	void RenderEngine::Render()
	{
		HZ_SCOPE_PERF("RenderEngine::Render()");
		for (auto& worldRenderer : WorldRenderer::s_Renderers)
		{
			worldRenderer->Invalidate();
		}
		m_RenderCommandBuffer->Begin();

		for (auto& worldRenderer : WorldRenderer::s_Renderers)
		{
			if (!worldRenderer->IsValid()) continue;

			worldRenderer->Begin(m_RenderCommandBuffer, m_Queue);

			if (worldRenderer->m_Settings.Flags & WorldRenderFlags_::Enabled)
			{
				m_RenderPassData.ViewProjection = worldRenderer->m_Settings.Camera->GetViewPprojection();

				m_CurrentRenderPass = worldRenderer->GetRenderPass();
				if (worldRenderer->m_Settings.Flags & WorldRenderFlags_::Geometry)
				{
					m_Renderer2D->SetTargetRenderPass(m_CurrentRenderPass);
					m_DebugRenderer->SetTargetRenderPass(m_CurrentRenderPass);

					m_Renderer2D->BeginWorld(m_RenderPassData, (WorldRenderFlags_)worldRenderer->m_Settings.Flags);
					m_DebugRenderer->BeginWorld(m_RenderPassData, (WorldRenderFlags_)worldRenderer->m_Settings.Flags);

					m_Queue->Excecute();

					m_Renderer2D->EndWorld();
					m_DebugRenderer->EndWorld();

					m_PostPassQueue->Excecute();
				}
			}
			worldRenderer->End(m_RenderCommandBuffer);
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
		HZR_PROFILE_FUNCTION();
	}
	void RenderEngine::DrawGeometry(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Ref<Pipeline> pipeline)
	{
		vertexBuffer->Bind(m_RenderCommandBuffer);
		indexBuffer->Bind(m_RenderCommandBuffer);
		pipeline->Bind(m_RenderCommandBuffer);
		pipeline->Draw(m_RenderCommandBuffer, indexBuffer->GetCount());
	}
	void RenderEngine::DispatchPipeline(Ref<Pipeline> pipeline, uint32_t count) {
		pipeline->Bind(m_RenderCommandBuffer);
		pipeline->DrawArrays(m_RenderCommandBuffer, count);
	}
	void RenderEngine::SetLineWidth(float lineWidth)
	{
		RenderContextCommand::SetLineWidth(m_RenderCommandBuffer, lineWidth);
	}
	bool RenderEngine::OnResize(WindowResizeEvent& e)
	{
		return false;
	}
}


#include "GameViewport.h"
#include "Editor/EditorWorldManager.h"
#include <tuple>

using namespace HazardRenderer;

namespace UI
{
	GameViewport::GameViewport() : Panel("Game Viewport")
	{
		FrameBufferCreateInfo frameBufferInfo = {};
		frameBufferInfo.DebugName = "GameViewport";
		frameBufferInfo.SwapChainTarget = false;
		frameBufferInfo.AttachmentCount = 2;
		frameBufferInfo.ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		frameBufferInfo.Attachments = { { ImageFormat::RGBA, ImageFormat::Depth } };
		frameBufferInfo.AttachmentCount = 2;
		frameBufferInfo.Width = 1280;
		frameBufferInfo.Height = 720;

		m_FrameBuffer = FrameBuffer::Create(&frameBufferInfo);

		RenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.DebugName = frameBufferInfo.DebugName;
		renderPassInfo.pTargetFrameBuffer = m_FrameBuffer;

		m_RenderPass = RenderPass::Create(&renderPassInfo);
	}
	void GameViewport::Update()
	{
		HZR_PROFILE_FUNCTION();
		Ref<World> world = Editor::EditorWorldManager::GetWorldRender()->GetTargetWorld();
		if (!world) return;

		auto& [cc, tc] = world->GetWorldCamera();
		if (cc == nullptr) 
		{ 
			m_HasCamera = false;
			return; 
		};

		m_HasCamera = true;
		glm::mat4 view = tc->GetTransformNoScale();

		cc->RecalculateProjection(m_Width, m_Height);

		WorldCameraData cameraData = {};
		cameraData.Projection = cc->GetProjection();
		cameraData.View = view;
		cameraData.OutputFrameBuffer = m_FrameBuffer;
		cameraData.RenderPass = m_RenderPass;
		cameraData.Width = m_Width;
		cameraData.Height = m_Height;

		auto& renderer = Editor::EditorWorldManager::GetWorldRender();
		//renderer->SubmitCamera(cameraData);
	}
	void GameViewport::OnPanelRender()
	{
		HZR_PROFILE_FUNCTION();
		ImVec2 size = ImGui::GetContentRegionAvail();

		if (size.x != m_Width || size.y != m_Height)
		{
			m_Width = size.x;
			m_Height = size.y;
		}
		if (!m_HasCamera || true) return;

		//else ImUI::Image(m_FrameBuffer->GetImage(), size);
	}
	bool GameViewport::OnEvent(Event& e)
	{
		return false;
	}
}

#include "GameViewport.h"
#include "Editor/EditorWorldManager.h"
#include <tuple>

using namespace HazardRenderer;

namespace UI
{
	GameViewport::GameViewport() : Panel("Game Viewport")
	{
		FrameBufferCreateInfo frameBufferInfo = {};
		frameBufferInfo.DebugName = "ViewportCamera";
		frameBufferInfo.SwapChainTarget = false;
		frameBufferInfo.AttachmentCount = 2;
		frameBufferInfo.ClearColor = { 0.05f, 0.05f, 0.05f, 1.0f };
		frameBufferInfo.Attachments = { { ImageFormat::RGBA }, { ImageFormat::Depth } };
		frameBufferInfo.AttachmentCount = 2;
		frameBufferInfo.Width = m_Width;
		frameBufferInfo.Height = m_Height;

		m_FrameBuffer = FrameBuffer::Create(&frameBufferInfo);
		RenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.DebugName = frameBufferInfo.DebugName;
		renderPassInfo.pTargetFrameBuffer = m_FrameBuffer;

		m_RenderPass = RenderPass::Create(&renderPassInfo);
	}
	void GameViewport::Update()
	{
		Ref<World> world = Editor::EditorWorldManager::GetWorldRender()->GetTargetWorld();
		auto& [cc, tc] = world->GetWorldCamera();
		if (cc == nullptr) { 
			m_HasCamera = false;
			return; 
		};
		m_HasCamera = true;
		glm::mat4 view = tc->GetTransformNoScale();

		cc->RecalculateProjection(m_Width, m_Height);

		WorldCameraData cameraData = {};
		cameraData.ViewProjection = cc->GetProjection() * glm::inverse(view);
		cameraData.Projection = cc->GetProjection();
		cameraData.View = view;
		cameraData.Position = tc->Translation;
		cameraData.OutputFrameBuffer = m_FrameBuffer;
		cameraData.RenderPass = m_RenderPass;

		auto& renderer = Editor::EditorWorldManager::GetWorldRender();
		renderer->SubmitCamera(cameraData);
	}
	void GameViewport::OnPanelRender()
	{
		ImVec2 size = ImGui::GetContentRegionAvail();

		if (size.x != m_Width || size.y != m_Height)
		{
			m_Width = size.x;
			m_Height = size.y;
			m_FrameBuffer->Resize(m_Width, m_Height);
		}
		if (!m_HasCamera) return;

		ImUI::Image(m_FrameBuffer->GetImage(), size);
	}
	bool GameViewport::OnEvent(Event& e)
	{
		return false;
	}
}
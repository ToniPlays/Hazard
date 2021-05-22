
#include <hzreditor.h>
#include "GameViewport.h"
#include "GUI/Library/Layout/Layout.h"

using namespace Hazard;

namespace WindowElement {

	GameViewport::GameViewport() : EditorWindow(ICON_FK_GAMEPAD " Game") {}
	GameViewport::~GameViewport() {}
		
	void GameViewport::Init()
	{
		SetActive(true);

		Rendering::FrameBufferCreateInfo createInfo;
		createInfo.attachments = { FrameBufferTextureFormat::RGBA8, FrameBufferTextureFormat::Depth };

		m_RenderTexture = RenderUtils::Create<FrameBuffer>(createInfo);

		effect = RenderUtils::CreateRaw<VignetteEffect>("");
	}
	void GameViewport::OnWindowRender()
	{
		ECS::World& world = ECS::SceneCommand::GetCurrentWorld();
		auto&[found, cam, transform] = world.GetWorldCamera();

		if (!found) {
			const char* text = "No active camera";
			ImVec2 size = ImGui::CalcTextSize(text);
			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - size.x) / 2);
			ImGui::SetCursorPosY((ImGui::GetWindowHeight() - size.y) / 2);
			WindowLayout::Layout::Text(text);
			return;
		}

		Rendering::RenderCommand::SetFrameBuffer(m_RenderTexture);
		ECS::SceneCommand::RenderScene(Rendering::Camera(cam->GetProjection(), transform->GetTransformNoScale(), 
			transform->m_Translation));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImVec2 size = ImGui::GetContentRegionAvail();

		if (size.x != m_Width || size.y != m_Height) {
			m_Width = size.x;
			m_Height = size.y;

			m_RenderTexture->Resize(size.x, size.y);
			effect->GetTargetTexture().Resize(size.x, size.y);
			cam->RecalculateProjection(size.x, size.y);
		}

		effect->Process(m_RenderTexture.Raw(), { m_Width, m_Height });

		ImGui::Image((void*)effect->GetTargetTexture().GetColorID(0),
			size, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::PopStyleVar();
	}
}

#include <hzreditor.h>
#include "GameViewport.h"
#include "GUI/Library/Layout/Layout.h"
#include "GUI/Library/Input.h"

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

		VignetteEffectCreateInfo vignette;
		vignette.outer = 0.5f;
		vignette.inner = 0.3f;
		vignette.intensity = 0.4f;

		BloomCreateInfo bloom;
		bloom.threshold = 0.2f;

		PostProcessingStackCreateInfo info;
		info.vignette = &vignette;
		info.bloom = &bloom;

		m_PostProcessing = new PostProcessingStack(info);
	}
	void GameViewport::OnWindowRender()
	{

		Rendering::BloomEffect* bloom = m_PostProcessing->Get<BloomEffect>();

		if (bloom) {
			Input::Slider("Bloom threshold", bloom->threshold, 0, 1);
			Input::Slider("Bloom intensity", bloom->intensity, 0, 1);
		}

		Rendering::RenderCommand::SetFrameBuffer(m_RenderTexture.Raw());
		ECS::World& world = ECS::WorldCommand::GetCurrentWorld();
		auto&[found, cam, transform] = world.GetWorldCamera();

		if (!found) {
			const char* text = "No active camera";
			ImVec2 size = ImGui::CalcTextSize(text);
			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - size.x) / 2);
			ImGui::SetCursorPosY((ImGui::GetWindowHeight() - size.y) / 2);
			WindowLayout::Layout::Text(text);
			return;
		}

		ECS::WorldCommand::RenderScene(Rendering::Camera(cam->GetProjection(), transform->GetTransformNoScale(),
			transform->m_Translation));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImVec2 size = ImGui::GetContentRegionAvail();

		if (size.x != m_Width || size.y != m_Height) {
			m_Width = size.x;
			m_Height = size.y;

			m_RenderTexture->Resize(size.x, size.y);
			m_PostProcessing->Resize(size.x, size.y);
			cam->RecalculateProjection(size.x, size.y);
		}

		FrameBuffer* result = m_PostProcessing->PostProcess(m_RenderTexture.Raw(), { m_Width, m_Height });

		ImGui::Image((void*)result->GetColorID(),
			size, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::PopStyleVar();
	}
}

#include "GameViewport.h"
#include "Library/Layout/Layout.h"
#include "Library/Input.h"
#include "Hazard/Entity/WorldCommand.h"

using namespace Hazard;
using namespace WindowLayout;

namespace WindowElement {

	GameViewport::GameViewport() : EditorWindow(ICON_FK_GAMEPAD " Game") 
	{
	}
		
	void GameViewport::Init()
	{
		// SetActive(false);
		// if (!IsActive()) return;

		// WorldRendererSettings settings = {};
		// settings.ViewportSize = { 1920, 1080 };
		// settings.Camera = &m_Camera;
		// settings.Flags = WorldRenderFlags_::Geometry | WorldRenderFlags_::Quads;
		// settings.ClearColor = Color::FromHex("#646464");
		// m_Renderer = WorldRenderer::Create(&settings);
	}
	void GameViewport::OnUpdate()
	{
		Ref<ECS::World> world = ECS::WorldCommand::GetCurrentWorld();
		const auto& [cam, transform] = world->GetWorldCamera();

		if (cam) 
		{
			if (m_Resize) {
				cam->RecalculateProjection(m_Width, m_Height);
			}

			m_Camera.SetView(glm::inverse(transform->GetTransformNoScale()));
			m_Camera.SetProjection(cam->GetProjection());
		}
	}
	void GameViewport::OnWindowRender()
	{
		if (!m_Renderer->IsValid()) {
			const char* text = "No active camera";
			ImVec2 size = ImGui::CalcTextSize(text);
			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - size.x) / 2);
			ImGui::SetCursorPosY((ImGui::GetWindowHeight() - size.y) / 2);
			WindowLayout::Layout::Text(text);
			return;
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImVec2 size = ImGui::GetContentRegionAvail();

		if (size.x != m_Width || size.y != m_Height) 
		{
			m_Width = size.x;
			m_Height = size.y;

			m_Renderer->SetViewport(m_Width, m_Height);
			m_Resize = true;
		}

		Layout::Image(m_Renderer->GetFinalPassImage(), size, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::PopStyleVar();
	}
}
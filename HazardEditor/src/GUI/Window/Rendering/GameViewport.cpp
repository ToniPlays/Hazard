
#include "GameViewport.h"
#include "Library/Layout/Layout.h"
#include "Library/Input.h"

using namespace Hazard;
using namespace WindowLayout;

namespace WindowElement {

	GameViewport::GameViewport() : EditorWindow(ICON_FK_GAMEPAD " Game") {}
		
	void GameViewport::Init()
	{
		SetActive(Application::HasModule<RenderEngine>());
		if (!IsActive()) return;

		WorldRendererSettings settings = {};
		settings.Flags = WorldRenderFlags_::Enabled | WorldRenderFlags_::Geometry;
		settings.ClearColor = Color::FromHex("#646464");
		m_Renderer = WorldRenderer::Create(&settings);
	}
	void GameViewport::OnWindowRender()
	{
		Ref<ECS::World> world = ECS::WorldCommand::GetCurrentWorld();
		auto&[cam, transform] = world->GetWorldCamera();

		if (!cam) {
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

			m_Renderer->SetViewport(size.x, size.y);
		}

		Layout::Image(m_Renderer->GetFinalPassImage(), size, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::PopStyleVar();
	}
}

#include <hzreditor.h>
#include "GameViewport.h"
#include "GUI/Library/Layout/Layout.h"

using namespace Hazard;

namespace WindowElement {

	GameViewport::GameViewport() : EditorWindow(ICON_FK_GAMEPAD " Game")
	{

	}
	GameViewport::~GameViewport()
	{

	}
	void GameViewport::Init()
	{
		SetActive(true);
		renderTexture = RenderUtils::Create<RenderTexture>();
	}
	void GameViewport::OnWindowRender()
	{
		Hazard::ECS::Scene& scene = ECS::SceneCommand::GetCurrentScene();
		auto&[found, cam, transform] = scene.GetSceneCamera();

		if (!found) {
			const char* text = "No active camera";
			ImVec2 size = ImGui::CalcTextSize(text);
			ImGui::SetCursorPosX((ImGui::GetWindowWidth() - size.x) / 2);
			ImGui::SetCursorPosY((ImGui::GetWindowHeight() - size.y) / 2);
			WindowLayout::Layout::Text(text);
			return;
		}

		Rendering::RenderCommand::SetRenderTarget(renderTexture);
		ECS::SceneCommand::RenderScene(Rendering::Camera(cam->GetProjection(), transform->GetTransformNoScale(), 
			transform->Translation, cam->bgRenderer));

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });
		ImVec2 size = ImGui::GetContentRegionAvail();

		if (size.x != width || size.y != height) {
			width = size.x;
			height = size.y;

			renderTexture->Resize(size.x, size.y);
			cam->RecalculateProjection(size.x, size.y);
		}

		ImGui::Image((void*)renderTexture->GetColorID(),
			size, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::PopStyleVar();
	}
}
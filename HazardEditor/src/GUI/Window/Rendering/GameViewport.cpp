
#include <hzreditor.h>
#include "GameViewport.h"
#include "GUI/Library/Layout.h"

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
			WindowLayout::Layout::Text("No active camera");
			return;
		}
		Rendering::RenderCommand::SetRenderTarget(renderTexture);
		ECS::SceneCommand::RenderScene(cam.projection * glm::inverse(transform.GetTransformMat4()), cam.bgColor.ToGlm());

		ImVec2 size = ImGui::GetContentRegionAvail();

		if (size.x != width || size.y != height) {
			width = size.x;
			height = size.y;

			renderTexture->Resize(width, height);
			cam.RecalculateProjection(width, height);
		}
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

		ImGui::Image((void*)renderTexture->GetColorID(),
			size, ImVec2(0, 1), ImVec2(1, 0));

		ImGui::PopStyleVar();
	}
}
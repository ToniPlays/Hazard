
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
		bool found = false;
		renderer = &Application::GetModule<RenderEngine>(found);
		SetActive(found);

		renderTexture = RenderUtils::Create<RenderTexture>();

		if (found) renderer->SetRenderTarget(RenderUtils::Create<RenderTexture>());
	}
	void GameViewport::OnWindowRender()
	{
		Hazard::ECS::Scene& scene = ECS::SceneCommand::GetCurrentScene();
		renderer->SetRenderTarget(renderTexture);

		auto&[found, cam] = scene.GetSceneCamera();
		if (!found) return;

		renderer->SceneRender(scene, cam);

		ImVec2 size = ImGui::GetContentRegionAvail();

		if (size.x != width || size.y != height) {
			width = size.x;
			height = size.y;

			renderTexture->Resize(width, height);
			cam.component.RecalculateProjection(width, height);
		}
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0, 0 });

		ImGui::Image((void*)renderTexture->GetColorID(),
			size, ImVec2(0, 1), ImVec2(1, 0));

		ImGui::PopStyleVar();
	}
}
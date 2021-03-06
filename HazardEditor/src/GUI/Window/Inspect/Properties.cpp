#pragma once

#include <hzreditor.h>
#include "Properties.h"
#include "GUI/Library/Input.h"
#include "GUI/Library/Layout.h"
#include "GUI/Library/Style.h"
#include "GUI/Library/ComponentDraw.h"

using namespace WindowLayout;

namespace WindowElement {

	Properties::Properties() : EditorWindow("Properties")
	{

	}
	Properties::~Properties()
	{

	}
	void Properties::OnWindowRender()
	{
		if (!selectionContext.IsValid()) return;

		DrawComponent<TagComponent>(ICON_FK_TAG " Tag", selectionContext);
		DrawComponent<TransformComponent>(ICON_FK_WRENCH " Transform", selectionContext);
		DrawComponent<CameraComponent>(ICON_FK_CAMERA " Camera", selectionContext);
		DrawComponent<SpriteRendererComponent>(" Sprite", selectionContext);

		DrawComponent<SkyLightComponent>(ICON_FK_CLOUD " Sky light", selectionContext);
		DrawComponent<DirectionalLightComponent>(ICON_FK_SUN_O " Directional light", selectionContext);
		DrawComponent<PointLightComponent>(ICON_FK_LIGHTBULB_O " Point light", selectionContext);
		DrawComponent<BatchComponent>("Batch", selectionContext);


		Layout::ContextMenu([&]() {
			Layout::MenuItem("Camera", [&]() {
				selectionContext.AddComponent<CameraComponent>();
				});
			Layout::MenuItem("Sprite renderer", [&]() {
				selectionContext.AddComponent<SpriteRendererComponent>();
			});
			Layout::MenuItem("Camera", [&]() {
				selectionContext.AddComponent<CameraComponent>();
				});
			Layout::MenuItem("Sky Light", [&]() {
				selectionContext.AddComponent<SkyLightComponent>();
			});
			Layout::MenuItem("Directional light", [&]() {
				selectionContext.AddComponent<DirectionalLightComponent>();
				});
			Layout::MenuItem("Point light", [&]() {
				selectionContext.AddComponent<PointLightComponent>();
				});
			Layout::MenuItem("Batch renderer", [&]() {
				selectionContext.AddComponent<BatchComponent>();
				});
		});

	}
	bool Properties::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Events::SelectionContextChange>(BIND_EVENT(Properties::SelectionContextChange));
		return false;
	}
	bool Properties::SelectionContextChange(Events::SelectionContextChange& e)
	{
		if(!IsLocked()) selectionContext = e.GetEntity();
		return false;
	}
}
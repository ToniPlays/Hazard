#pragma once

#include <hzreditor.h>
#include "Inspector.h"
#include "GUI/Library/Input.h"
#include "GUI/Library/Layout.h"
#include "GUI/Library/Style.h"
#include "GUI/Library/ComponentDraw.h"

using namespace WindowLayout;

namespace WindowElement {

	Inspector::Inspector() : EditorWindow("Inspector")
	{

	}
	Inspector::~Inspector()
	{

	}
	void Inspector::OnWindowRender()
	{
		if (!selectionContext.IsValid()) return;

		DrawComponent<TagComponent>("Tag", selectionContext);
		DrawComponent<TransformComponent>("Transform", selectionContext);
		DrawComponent<CameraComponent>("Camera", selectionContext);
		DrawComponent<SpriteRendererComponent>("Sprite", selectionContext);

		DrawComponent<SkyLightComponent>("Sky light", selectionContext);
		DrawComponent<DirectionalLightComponent>("Directional light", selectionContext);
		DrawComponent<PointLightComponent>("Point light", selectionContext);
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
				selectionContext.AddComponent<SkyLightComponent>();
				});
			Layout::MenuItem("Point light", [&]() {
				selectionContext.AddComponent<PointLightComponent>();
				});
			Layout::MenuItem("Batch renderer", [&]() {
				selectionContext.AddComponent<BatchComponent>();
				});
		});

	}
	bool Inspector::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Events::SelectionContextChange>(BIND_EVENT(Inspector::SelectionContextChange));
		return false;
	}
	bool Inspector::SelectionContextChange(Events::SelectionContextChange& e)
	{
		if(!IsLocked()) selectionContext = e.GetEntity();
		return false;
	}
}
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
		if (!selectionContext) return;

		DrawComponent<TagComponent>("Tag", selectionContext);
		DrawComponent<TransformComponent>("Transform", selectionContext);
		DrawComponent<CameraComponent>("CameraComponent", selectionContext);
		
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
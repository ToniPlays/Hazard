
#include <hzrpch.h>
#include "EditorMainTab.h"
#include "Library/Layout/Dockspace.h"
#include "GUI/Window/AllWindows.h"
#include <imgui_internal.h>

using namespace WindowLayout;
namespace WindowElement
{
	void EditorMainTab::Init()
	{
		PushRenderable<GameViewport>();
		PushRenderable<Viewport>();
		PushRenderable<Properties>();
		PushRenderable<Hierarchy>();
		PushRenderable<Console>();
		//PushRenderable<FileView>();
		PushRenderable<MaterialInspector>();
	}
	void EditorMainTab::OnUpdate()
	{
		HZR_PROFILE_FUNCTION();
		for (RenderableElement* element : GetElements()) {
			element->OnUpdate();
		}
	}
	void EditorMainTab::Render()
	{
		HZR_PROFILE_FUNCTION();
		for (RenderableElement* element : GetElements()) {
			element->OnRender();
		}
	}
	bool EditorMainTab::OnEvent(Event& e)
	{
		for (RenderableElement* element : GetElements()) {
			if (element->OnEvent(e)) return true;
		}
		return false;
	}
}

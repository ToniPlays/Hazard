
#include "NodeGraphTab.h"
#include "GUI/Window/AllWindows.h"

namespace WindowElement
{
	void NodeGraphTab::Init()
	{

	}
	void NodeGraphTab::Render()
	{
		for (RenderableElement* element : GetElements()) {
			element->OnRender();
		}
	}
	bool NodeGraphTab::OnEvent(Event& e)
	{
		for (RenderableElement* element : GetElements()) {
			if (element->OnEvent(e)) return true;
		}
		return false;
	}
}


#include "Properties.h"
#include "ComponentUILibrary.h"


namespace UI 
{
	Properties::Properties() : Panel("Properties") {}

	void Properties::OnPanelRender() 
	{
		if (!m_SelectionContext.IsValid()) return;

		UI::ComponentMenuIfExists<TagComponent>(m_SelectionContext);
		UI::ComponentMenuIfExists<TransformComponent>(m_SelectionContext);

	}
	bool Properties::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		return dispatcher.Dispatch<Events::SelectionContextChange>(BIND_EVENT(Properties::OnSelectionContextChange));
	}
	bool Properties::OnSelectionContextChange(Events::SelectionContextChange& e)
	{
		m_SelectionContext = e.GetEntity();
		return true;
	}
}
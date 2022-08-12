
#include "ScriptCreatePanel.h"

namespace UI
{
	ScriptCreatePanel::ScriptCreatePanel() : Hazard::ImUI::Modal("Create script", { 600, 700 })
	{

	}
	bool ScriptCreatePanel::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		return dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT(ScriptCreatePanel::OnKeyPressed));
	}
	bool ScriptCreatePanel::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == Key::Escape) {
			Close();
			return true;
		}
		return false;
	}
	void ScriptCreatePanel::OnPanelRender()
	{

	}
}
#include "Panel.h"
#include "GUIManager.h"

namespace Hazard::ImUI
{
	void Panel::Render()
	{
		if (!m_IsOpen)
		{
			if (m_DestroyOnClose)
			{
				//Remove panel from list
				auto& manager = Application::Get().GetModule<GUIManager>();
				manager.Destroy(this);
			}
			return;
		}

		if (!ImGui::Begin(m_Title.c_str(), &m_IsOpen))
		{
			ImGui::End();
			return;
		}

		if (m_IsOpen)
		{
			m_Hovered = MouseOverWindow();
			OnPanelRender();
		}

		ImGui::End();
	}
}
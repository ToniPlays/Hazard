
#include "EditorTabWindow.h"
#include <imgui.h>

namespace WindowElement
{
	EditorTabWindow::~EditorTabWindow()
	{
		for (uint32_t i = 0; i < m_Elements.size(); i++) {
			delete m_Elements[i];
		}
		m_Elements.clear();
	}
	void EditorTabWindow::OnRender()
	{
		Render();
	}
}

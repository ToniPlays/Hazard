
#include "RenderCommandListPanel.h"

namespace UI 
{
	void RenderCommandListPanel::OnPanelRender() 
	{
		UI::Treenode("Some crap", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed, []() {
			ImGui::Text("Wello");
		});
	}
}
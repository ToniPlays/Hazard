
#include "Hierarchy.h"
#include <sstream>
namespace UI
{
	void Hierarchy::OnPanelRender()
	{
		//Draw hierarchy panel

		const char* columns[] = { "Name", "Type", "Modifiers" };

		UI::Table("Hierarchy", columns, 3, [&]() {

			for (uint32_t i = 0; i < 10; i++) {
				std::stringstream ss;
				ss << "Row" << i;

				UI::TableRow(ss.str().c_str(), i == 2);

				//Type
				ImGui::TableNextColumn();

				ShiftX(4.0f);
				ImGui::Text("Entity");

				//Modifiers
				ImGui::TableNextColumn();

				ScopedStyleVar frame(ImGuiStyleVar_FrameBorderSize, 0);
				ImVec4 col = StyleManager::GetCurrent().Window.Text;
				ScopedStyleColor textColor(ImGuiCol_Text, col);

				ShiftX(4.0f);
				ImGui::Text(ICON_FK_EYE);
			}
			UI::ContextMenu([]() {
				UI::MenuItem("Some shit", []() {});
				UI::MenuItem("Some more shit", []() {});
				UI::MenuItem("Some more Hazard", []() {});
				UI::Submenu("Crashes", []() {});
			});
		
		});
	}
}
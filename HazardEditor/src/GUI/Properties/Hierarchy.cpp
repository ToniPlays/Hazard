
#include "Hierarchy.h"
#include "Hazard.h"
#include "Core/EditorEvent.h"
#include <sstream>

namespace UI
{
	using namespace Hazard;

	Hierarchy::Hierarchy() : Panel("Hierarchy")
	{
		m_WorldHandler = &Hazard::Application::GetModule<Hazard::WorldHandler>();
	}
	void Hierarchy::OnPanelRender()
	{
		//Draw hierarchy panel
		Ref<World> world = m_WorldHandler->GetCurrentWorld();

		const char* columns[] = { "Name", "Type", "Modifiers" };

		UI::Table("Hierarchy", columns, 3, [&]() {

			for (auto entity : world->GetEntitiesWith<TagComponent>()) {
				Entity e(entity, world.Raw());
				TagComponent& tag = e.GetComponent<TagComponent>();

				bool clicked = UI::TableRowTreeItem(tag.Tag.c_str(), false, []() {
					ImGui::Text("Sup bro");
					});

				//Type
				ImGui::TableNextColumn();

				ShiftX(4.0f);
				ImGui::Text("Entity");

				//Modifiers
				ImGui::TableNextColumn();

				ScopedStyleVar frame(ImGuiStyleVar_FrameBorderSize, 0);
				ImVec4 col = StyleManager::GetCurrent().Window.Text;
				ScopedStyleColor textColor(ImGuiCol_Text, col);

				ShiftX(8.0f);
				ImGui::Text(ICON_FK_EYE);

				if (clicked) {
					Events::SelectionContextChange ev (e );
					Hazard::HazardLoop::GetCurrent().OnEvent(ev);
				}
			}

			UI::ContextMenu([&]() {
				UI::MenuItem("Create new", [&]() {
					world->CreateEntity("New entity");
					});
				});
			});
	}
}

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

				bool clicked = UI::TableRowTreeItem(tag.Tag.c_str(), e == m_SelectionContext, []() {
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

				DrawModifiers(e, tag);

				if (clicked) {
					m_SelectionContext = e;
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
	void Hierarchy::DrawModifiers(Entity& e, TagComponent& tag)
	{
		const char* modifiers[] = { tag.Visible ? ICON_FK_EYE : ICON_FK_EYE_SLASH, ICON_FK_FILE_CODE_O};
		const bool states[] = { true, e.HasComponent<ScriptComponent>() };

		const ImVec4 visibleColor = StyleManager::GetCurrent().Colors.AxisZ;
		const ImVec4 textColor = StyleManager::GetCurrent().Window.Text;
		const ImVec4 warning = StyleManager::GetCurrent().Colors.Warning;

		const ImVec4 colors[] = { tag.Visible ? visibleColor : textColor, warning };

		for (uint32_t i = 0; i < 2; i++) {
			if (states[i]) {
				ScopedStyleColor col(ImGuiCol_Text, colors[i]);
				ShiftX(8.0f);
				ImGui::Text(modifiers[i]);
				ImGui::SameLine();
			}
		}
	}
}

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
					Events::SelectionContextChange ev(e);
					Hazard::HazardLoop::GetCurrent().OnEvent(ev);
				}
			}
			DrawContextMenu(world);

			});
		if (ImGui::IsItemClicked()) {
			m_SelectionContext = {};
			Events::SelectionContextChange ev({});
			Hazard::HazardLoop::GetCurrent().OnEvent(ev);
		}

	}
	void Hierarchy::DrawModifiers(Entity& e, TagComponent& tag)
	{
		bool scriptState = false;
		if (e.HasComponent<ScriptComponent>()) {
			ScriptEngine& engine = Application::GetModule<ScriptEngine>();
			auto& sc = e.GetComponent<ScriptComponent>();
			scriptState = !engine.HasModule(sc.ModuleName);
		}
		


		const char* modifiers[] = { tag.Visible ? ICON_FK_EYE : ICON_FK_EYE_SLASH, ICON_FK_FILE_CODE_O };
		const char* tooltips[] = { tag.Visible ? "Visible" : "Hidden", "Script is missing" };
		const bool states[] = { true, scriptState };

		const ImVec4 visibleColor = StyleManager::GetCurrent().Colors.AxisZ;
		const ImVec4 textColor = StyleManager::GetCurrent().Window.Text;
		const ImVec4 warning = StyleManager::GetCurrent().Colors.Warning;

		const ImVec4 colors[] = { tag.Visible ? visibleColor : textColor, warning };

		for (uint32_t i = 0; i < 2; i++) {
			if (states[i]) {
				{
					ScopedStyleColor col(ImGuiCol_Text, colors[i]);
					ShiftX(8.0f);
					ImGui::Text(modifiers[i]);
				}
				UI::Tooltip(tooltips[i]);
				ImGui::SameLine();
			}
		}
	}
	void Hierarchy::DrawContextMenu(Ref<World>& world)
	{
		const Style& style = StyleManager::GetCurrent();
		UI::ContextMenu([&]() {
			UI::MenuItem("Create empty", [&]() {
				world->CreateEntity("New entity");
				});

			Separator({ImGui::GetContentRegionAvailWidth(), 2.0f}, style.Window.HeaderActive);

			UI::Submenu("3D", [&]() {
				UI::MenuItem("Cube", [&]() {
					auto& entity = world->CreateEntity("New Cube");
					entity.AddComponent<MeshComponent>();
					});
				UI::MenuItem("Plane", [&]() {
					auto& entity = world->CreateEntity("New Plane");
					entity.AddComponent<MeshComponent>();
					});
				UI::MenuItem("Mesh", [&]() {
					auto& entity = world->CreateEntity("New ;esh");
					entity.AddComponent<MeshComponent>();
					});
				});
			UI::Submenu("2D", [&]() {
				UI::MenuItem("Sprite", [&]() {
					auto& entity = world->CreateEntity("New Sprite");
					entity.AddComponent<SpriteRendererComponent>();
					});
				});
			UI::Submenu("Lighting", [&]() {
				UI::MenuItem("Sky light", [&]() {
					auto& entity = world->CreateEntity("Sky light");
					entity.AddComponent<SkyLightComponent>();
					});
				UI::MenuItem("Directional light", [&]() {
					auto& entity = world->CreateEntity("New Directional light");
					entity.AddComponent<DirectionalLightComponent>();
					});
				UI::MenuItem("Point light", [&]() {
					auto& entity = world->CreateEntity("New Point light");
					entity.AddComponent<PointLightComponent>();
					});
				});
			UI::Submenu("Audio", [&]() {
				UI::MenuItem("Speaker", [&]() {
					auto& entity = world->CreateEntity("New Speaker");
					});
				UI::MenuItem("Audio listener", [&]() {
					auto& entity = world->CreateEntity("New Audio listener");
					});
				});

			UI::Submenu("UI", [&]() {

				});
			});
	}
}
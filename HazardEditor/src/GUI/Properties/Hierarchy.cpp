
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

		ImUI::Table("Hierarchy", columns, 3, [&]() {

			for (auto entity : world->GetEntitiesWith<TagComponent>()) {
				Entity e(entity, world.Raw());
				TagComponent& tag = e.GetComponent<TagComponent>();

				bool clicked = ImUI::TableRowTreeItem(tag.Tag.c_str(), e == m_SelectionContext, []() {
					ImGui::Text("Sup bro");
					});

				//Type
				ImGui::TableNextColumn();

				ImUI::ShiftX(4.0f);
				ImGui::Text("Entity");

				//Modifiers
				ImGui::TableNextColumn();

				ImUI::ScopedStyleVar frame(ImGuiStyleVar_FrameBorderSize, 0);
				ImVec4 col = ImUI::StyleManager::GetCurrent().Window.Text;
				ImUI::ScopedStyleColor textColor(ImGuiCol_Text, col);

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
		bool spriteState = false;
		bool isSkyLight = e.HasComponent<SkyLightComponent>();

		if (e.HasComponent<ScriptComponent>()) {
			ScriptEngine& engine = Application::GetModule<ScriptEngine>();
			auto& sc = e.GetComponent<ScriptComponent>();
			scriptState = !engine.HasModule(sc.ModuleName);
		}

		if (e.HasComponent<SpriteRendererComponent>()) {
			spriteState = !e.GetComponent<SpriteRendererComponent>().Texture;
		}
		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		const ImVec4 visibleColor = style.Colors.AxisZ;
		const ImVec4 textColor = style.Window.Text;
		const ImVec4 warning = style.Colors.Warning;
		

		const char* modifiers[] = { tag.Visible ? ICON_FK_EYE : ICON_FK_EYE_SLASH, ICON_FK_GLOBE, ICON_FK_FILE_CODE_O, ICON_FK_PICTURE_O };
		const char* tooltips[] = { tag.Visible ? "Visible" : "Hidden", "Skylight is included in this entity", "Script is missing", "Texture missing"};
		const bool states[] = { true, isSkyLight, scriptState, spriteState };
		const ImVec4 colors[] = { tag.Visible ? visibleColor : textColor, textColor , warning, warning };

		for (uint32_t i = 0; i < sizeof(states); i++) {
			if (states[i]) {
				{
					ImUI::ScopedStyleColor col(ImGuiCol_Text, colors[i]);
					ImUI::ShiftX(8.0f);
					ImGui::Text(modifiers[i]);
				}
				ImUI::Tooltip(tooltips[i]);
				ImGui::SameLine();
			}
		}
	}
	void Hierarchy::DrawContextMenu(Ref<World>& world)
	{
		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		ImUI::ContextMenu([&]() {
			ImUI::MenuItem("Create empty", [&]() {
				world->CreateEntity("New entity");
				});

			ImUI::Separator({ImGui::GetContentRegionAvailWidth(), 2.0f}, style.Window.HeaderActive);

			ImUI::Submenu("3D", [&]() {
				ImUI::MenuItem("Cube", [&]() {
					auto& entity = world->CreateEntity("New Cube");
					entity.AddComponent<MeshComponent>();
					});
				ImUI::MenuItem("Plane", [&]() {
					auto& entity = world->CreateEntity("New Plane");
					entity.AddComponent<MeshComponent>();
					});
				ImUI::MenuItem("Mesh", [&]() {
					auto& entity = world->CreateEntity("New ;esh");
					entity.AddComponent<MeshComponent>();
					});
				});
			ImUI::Submenu("2D", [&]() {
				ImUI::MenuItem("Sprite", [&]() {
					auto& entity = world->CreateEntity("New Sprite");
					entity.AddComponent<SpriteRendererComponent>();
					});
				});
			ImUI::Submenu("Lighting", [&]() {
				ImUI::MenuItem("Sky light", [&]() {
					auto& entity = world->CreateEntity("Sky light");
					entity.AddComponent<SkyLightComponent>();
					});
				ImUI::MenuItem("Directional light", [&]() {
					auto& entity = world->CreateEntity("New Directional light");
					entity.AddComponent<DirectionalLightComponent>();
					});
				ImUI::MenuItem("Point light", [&]() {
					auto& entity = world->CreateEntity("New Point light");
					entity.AddComponent<PointLightComponent>();
					});
				});
			ImUI::Submenu("Audio", [&]() {
				ImUI::MenuItem("Speaker", [&]() {
					auto& entity = world->CreateEntity("New Speaker");
					});
				ImUI::MenuItem("Audio listener", [&]() {
					auto& entity = world->CreateEntity("New Audio listener");
					});
				});

			ImUI::Submenu("UI", [&]() {

				});
			});
	}
}
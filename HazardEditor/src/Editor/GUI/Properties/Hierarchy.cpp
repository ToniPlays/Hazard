
#include "Hierarchy.h"
#include "Hazard.h"
#include "Core/EditorEvent.h"
#include "Editor/EditorWorldManager.h"
#include <sstream>

namespace UI
{
	using namespace Hazard;

	Hierarchy::Hierarchy() : Panel("Hierarchy", false)
	{
		m_SearchField = ImUI::TextField("");
		m_SearchField.SetIcon((const char*)ICON_FK_SEARCH);
		m_SearchField.SetHint("Search...");
	}

	void Hierarchy::Update()
	{
		HZR_PROFILE_FUNCTION();
	}

	void Hierarchy::OnPanelRender()
	{
		HZR_PROFILE_FUNCTION();
		//Draw hierarchy panel
		Ref<World> world = Editor::EditorWorldManager::GetWorldRender()->GetTargetWorld();
		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
        
        auto& metadata = AssetManager::GetMetadata(world->GetHandle());
		m_SearchField.Render();

		ImUI::Table_OLD("Hierarchy", { "Name", "Type", "Modifiers" }, ImGui::GetContentRegionAvail(), [&]() {

			for (auto& entity : world->GetEntitiesWith<TagComponent>())
			{
				Entity e(entity, world.Raw());
				TagComponent& tag = e.GetComponent<TagComponent>();

				if (!StringUtil::Contains(tag.Tag, m_SearchField.GetValue())) continue;

				bool isSelected = std::find(m_SelectionContext.begin(), m_SelectionContext.end(), e) != m_SelectionContext.end();
				ImUI::ScopedStyleColor color(ImGuiCol_Header, style.Window.HeaderHighlighted);

				bool clicked = ImUI::TableRowTreeItem(std::to_string(tag.Uid).c_str(), tag.Tag.c_str(), isSelected, []() {

				});

				/*ImUI::DragSource<UID>("Hazard.Entity", &e.GetUID(), [&]() {
					ImGui::Text(tag.Tag.c_str());
					ImGui::Text("Entity");
					});
					*/

					//Type
				ImGui::TableNextColumn();

				ImUI::ShiftX(4.0f);
				ImGui::Text("Entity");

				//Modifiers
				ImGui::TableNextColumn();

				ImUI::ScopedStyleVar frame(ImGuiStyleVar_FrameBorderSize, 0);
				ImVec4 col = style.Window.Text;
				ImUI::ScopedStyleColor textColor(ImGuiCol_Text, col);

				DrawModifiers(e, tag);

				if (clicked)
					SelectEntity(e);
			}
			DrawContextMenu(world);
		});

		if (ImGui::IsItemClicked())
			ClearSelected();
	}
	bool Hierarchy::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		bool handled = dispatcher.Dispatch<Events::SelectionContextChange>(BIND_EVENT(Hierarchy::OnEntitiesSelected));

		if (!m_Hovered) return false;

		handled |= dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT(Hierarchy::OnKeyPressed));

		return handled;
	}
	bool Hierarchy::OnKeyPressed(KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
			case Key::Delete:
			{
				std::vector<Entity> selections = m_SelectionContext;
				Ref<World> world = Editor::EditorWorldManager::GetWorldRender()->GetTargetWorld();
				for (auto& entity : selections)
					world->DestroyEntity(entity);
				ClearSelected();
				return true;
			}
			case Key::D:
			{
				std::vector<Entity> selections = m_SelectionContext;
				ClearSelected();
				Ref<World> world = Editor::EditorWorldManager::GetWorldRender()->GetTargetWorld();

				for (auto& entity : selections)
				{
					Entity e = world->CreateEntity(entity);
					SelectEntity(e);
				}
				return true;
			}
		}
		return false;
	}
	bool Hierarchy::OnEntitiesSelected(Events::SelectionContextChange& e)
	{
		m_SelectionContext = e.GetEntitites();
		return false;
	}

	void Hierarchy::DrawModifiers(Entity& e, TagComponent& tag)
	{
		bool scriptState = false;
		bool spriteState = false;
		bool isSkyLight = e.HasComponent<SkyLightComponent>();

		if (e.HasComponent<ScriptComponent>())
		{
			ScriptEngine& engine = Application::Get().GetModule<ScriptEngine>();
			auto& sc = e.GetComponent<ScriptComponent>();
			scriptState = !engine.FindModule(sc.ModuleName);
		}

		if (e.HasComponent<SpriteRendererComponent>())
			spriteState = !e.GetComponent<SpriteRendererComponent>().TextureHandle;

		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		const ImVec4 visibleColor = style.Colors.AxisZ;
		const ImVec4 textColor = style.Window.Text;
		const ImVec4 warning = style.Colors.Warning;


		const char* modifiers[] = { e.IsVisible() ? (const char*)ICON_FK_EYE : (const char*)ICON_FK_EYE_SLASH, (const char*)ICON_FK_GLOBE, (const char*)ICON_FK_FILE_CODE_O, (const char*)ICON_FK_PICTURE_O };
		const char* tooltips[] = { e.IsVisible() ? "Visible" : "Hidden", "Skylight is included in this entity", "Script is missing", "Texture missing" };
		const bool states[] = { true, isSkyLight, scriptState, spriteState };
		const ImVec4 colors[] = { e.IsVisible() ? visibleColor : textColor, textColor , warning, warning };

		for (uint32_t i = 0; i < sizeof(states); i++)
		{
			if (states[i])
			{
				{
					ImUI::ScopedStyleColor col(ImGuiCol_Text, colors[i]);
					ImUI::ShiftX(8.0f);
					ImGui::Text("%s", modifiers[i]);
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
				auto entity = world->CreateEntity("New entity");
				Events::SelectionContextChange e({ entity });
				Hazard::HazardLoop::GetCurrent().OnEvent(e);
			});

			ImUI::Separator({ ImGui::GetContentRegionAvail().x, 2.0f }, style.Window.HeaderActive);

			ImUI::MenuItem("Camera", [&]() {
				auto entity = world->CreateEntity("New camera");
				entity.AddComponent<CameraComponent>();
				SelectEntity(entity);
			});

			ImUI::Submenu("3D", [&]() {
				ImUI::MenuItem("Cube", [&]() {
					auto entity = world->CreateEntity("New Cube");
					SelectEntity(entity);
				});
				ImUI::MenuItem("Plane", [&]() {
					auto entity = world->CreateEntity("New Plane");
					entity.AddComponent<MeshComponent>();
					SelectEntity(entity);
				});
				ImUI::MenuItem("Mesh", [&]() {
					auto entity = world->CreateEntity("New mesh");
					entity.AddComponent<MeshComponent>();
					SelectEntity(entity);
				});
			});
			ImUI::Submenu("2D", [&]() {
				ImUI::MenuItem("Sprite", [&]() {
					auto entity = world->CreateEntity("New Sprite");
					entity.AddComponent<SpriteRendererComponent>();
					SelectEntity(entity);
				});
			});
			ImUI::Submenu("Lighting", [&]() {
				ImUI::MenuItem("Sky light", [&]() {
					auto entity = world->CreateEntity("Sky light");
					entity.AddComponent<SkyLightComponent>();
					SelectEntity(entity);
				});
				ImUI::MenuItem("Directional light", [&]() {
					auto entity = world->CreateEntity("New Directional light");
					entity.AddComponent<DirectionalLightComponent>();
					SelectEntity(entity);
				});
				ImUI::MenuItem("Point light", [&]() {
					auto entity = world->CreateEntity("New Point light");
					entity.AddComponent<PointLightComponent>();
					SelectEntity(entity);
				});
			});
			ImUI::Submenu("Audio", [&]() {
				ImUI::MenuItem("Speaker", [&]() {
					auto entity = world->CreateEntity("New Speaker");
					SelectEntity(entity);
				});
				ImUI::MenuItem("Audio listener", [&]() {
					auto entity = world->CreateEntity("New Audio listener");
					SelectEntity(entity);
				});
			});

			ImUI::Submenu("UI", [&]() {

			});
		});
	}
	void Hierarchy::SelectEntity(const Entity& entity)
	{
		if (!Input::IsKeyDown(Key::LeftControl))
			m_SelectionContext.clear();
		else
		{
			auto it = std::find(m_SelectionContext.begin(), m_SelectionContext.end(), entity);
			if (it != m_SelectionContext.end()) return;
		}

		m_SelectionContext.push_back(entity);
		Events::SelectionContextChange ev(m_SelectionContext);
		Hazard::HazardLoop::GetCurrent().OnEvent(ev);
	}
	void Hierarchy::DeselectEntity(const Entity& entity)
	{
		auto it = std::find(m_SelectionContext.begin(), m_SelectionContext.end(), entity);
		if (it != m_SelectionContext.end())
			m_SelectionContext.erase(it);

		Events::SelectionContextChange ev(m_SelectionContext);
		Hazard::HazardLoop::GetCurrent().OnEvent(ev);
	}
	void Hierarchy::ClearSelected()
	{
		m_SelectionContext.clear();
		Events::SelectionContextChange ev(m_SelectionContext);
		Hazard::HazardLoop::GetCurrent().OnEvent(ev);
	}
}
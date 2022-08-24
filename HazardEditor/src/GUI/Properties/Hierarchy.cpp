
#include "Hierarchy.h"
#include "Hazard.h"
#include "Core/EditorEvent.h"
#include "Hazard/Rendering/HRenderer.h"
#include "Editor/EditorWorldManager.h"
#include <sstream>

namespace UI
{
	using namespace Hazard;

	Hierarchy::Hierarchy() : Panel("Hierarchy")
	{
		m_WorldHandler = &Hazard::Application::GetModule<Hazard::WorldHandler>();
	}
	void Hierarchy::Update()
	{
		Ref<World> world = m_WorldHandler->GetCurrentWorld();

		auto& renderer = Editor::EditorWorldManager::GetWorldRender();
		renderer->SetTargetWorld(world);

		renderer->SubmitExtra([=]() mutable {
			auto& view = world->GetEntitiesWith<CameraComponent>();
			for (auto entity : view) {
				Entity e = { entity, world.Raw() };
				auto& tc = e.GetComponent<TransformComponent>();
				auto& cc = e.GetComponent<CameraComponent>();
				if (cc.GetProjectionType() == Projection::Perspective)
					HRenderer::DrawPerspectiveCameraFrustum(tc.Translation, tc.GetOrientation(), tc.GetTransformMat4(), cc.GetFov(), cc.GetClipping(), cc.GetAspectRatio(), Color::Green);
				else HRenderer::DrawOrthoCameraFrustum(tc.Translation, tc.GetOrientation(), tc.GetTransformMat4(), cc.GetSize(), cc.GetClipping(), cc.GetAspectRatio(), Color::Green);
			}
			});
	}
	void Hierarchy::OnPanelRender()
	{
		//Draw hierarchy panel
		Ref<World> world = m_WorldHandler->GetCurrentWorld();

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		ImUI::TextFieldWithHint(m_SearchValue, "Search...");

		const char* columns[] = { "Name", "Type", "Modifiers" };

		ImUI::Table("Hierarchy", columns, 3, [&]() {

			for (auto entity : world->GetEntitiesWith<TagComponent>()) {
				Entity e(entity, world.Raw());
				TagComponent& tag = e.GetComponent<TagComponent>();
				if (!StringUtil::Contains(tag.Tag, m_SearchValue)) continue;

				bool clicked = ImUI::TableRowTreeItem(tag.Tag.c_str(), e == m_SelectionContext, []() {
					ImGui::Text("Sup bro");
					});

				ImUI::DragSource<UID>("Hazard.Entity", &e.GetUID(), [&]() {
					ImGui::Text(tag.Tag.c_str());
					ImGui::Text("Entity");
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
					SelectEntity(e);
				}
			}
			DrawContextMenu(world);

			});
		if (ImGui::IsItemClicked()) {
			SelectEntity({});
		}
	}
	bool Hierarchy::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		return dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT(Hierarchy::OnKeyPressed));
	}
	bool Hierarchy::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == Key::Delete)
		{
			if (m_SelectionContext)
			{
				m_WorldHandler->GetCurrentWorld()->DestroyEntity(m_SelectionContext);
				SelectEntity({});
				return true;
			}
		}
		return false;
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


		const char* modifiers[] = { e.IsVisible() ? ICON_FK_EYE : ICON_FK_EYE_SLASH, ICON_FK_GLOBE, ICON_FK_FILE_CODE_O, ICON_FK_PICTURE_O };
		const char* tooltips[] = { e.IsVisible() ? "Visible" : "Hidden", "Skylight is included in this entity", "Script is missing", "Texture missing" };
		const bool states[] = { true, isSkyLight, scriptState, spriteState };
		const ImVec4 colors[] = { e.IsVisible() ? visibleColor : textColor, textColor , warning, warning };

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
				Entity e = world->CreateEntity("New entity");

				});

			ImUI::Separator({ ImGui::GetContentRegionAvailWidth(), 2.0f }, style.Window.HeaderActive);

			ImUI::MenuItem("Camera", [&]() {
				auto& entity = world->CreateEntity("New camera");
				entity.AddComponent<CameraComponent>();
				SelectEntity(entity);
				});

			ImUI::Submenu("3D", [&]() {
				ImUI::MenuItem("Cube", [&]() {
					auto& entity = world->CreateEntity("New Cube");
					entity.AddComponent<MeshComponent>();
					SelectEntity(entity);
					});
				ImUI::MenuItem("Plane", [&]() {
					auto& entity = world->CreateEntity("New Plane");
					entity.AddComponent<MeshComponent>();
					SelectEntity(entity);
					});
				ImUI::MenuItem("Mesh", [&]() {
					auto& entity = world->CreateEntity("New mesh");
					entity.AddComponent<MeshComponent>();
					SelectEntity(entity);
					});
				});
			ImUI::Submenu("2D", [&]() {
				ImUI::MenuItem("Sprite", [&]() {
					auto& entity = world->CreateEntity("New Sprite");
					entity.AddComponent<SpriteRendererComponent>();
					SelectEntity(entity);
					});
				});
			ImUI::Submenu("Lighting", [&]() {
				ImUI::MenuItem("Sky light", [&]() {
					auto& entity = world->CreateEntity("Sky light");
					entity.AddComponent<SkyLightComponent>();
					SelectEntity(entity);
					});
				ImUI::MenuItem("Directional light", [&]() {
					auto& entity = world->CreateEntity("New Directional light");
					entity.AddComponent<DirectionalLightComponent>();
					SelectEntity(entity);
					});
				ImUI::MenuItem("Point light", [&]() {
					auto& entity = world->CreateEntity("New Point light");
					entity.AddComponent<PointLightComponent>();
					SelectEntity(entity);
					});
				});
			ImUI::Submenu("Audio", [&]() {
				ImUI::MenuItem("Speaker", [&]() {
					auto& entity = world->CreateEntity("New Speaker");
					SelectEntity(entity);
					});
				ImUI::MenuItem("Audio listener", [&]() {
					auto& entity = world->CreateEntity("New Audio listener");
					SelectEntity(entity);
					});
				});

			ImUI::Submenu("UI", [&]() {

				});
			});
	}
	void Hierarchy::SelectEntity(const Entity& entity)
	{
		m_SelectionContext = entity;
		Events::SelectionContextChange ev(entity);
		Hazard::HazardLoop::GetCurrent().OnEvent(ev);
	}
}
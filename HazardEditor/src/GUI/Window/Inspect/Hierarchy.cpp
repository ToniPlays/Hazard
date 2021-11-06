#pragma once


#include "Hierarchy.h"
#include "GUI/EditorView.h"

#include "Library/Input.h"
#include "Library/Style.h"
#include "Library/Layout/Layout.h"
#include "Library/Layout/ContextMenus.h"
#include "GUI/Window/Rendering/WorldEnvironmentData.h"
#include "GUI/Window/DragDropUtils.h"

using namespace WindowLayout;

namespace WindowElement {

	Hierarchy::Hierarchy() : EditorWindow(ICON_FK_ALIGN_LEFT " Hierarchy") {}

	bool Hierarchy::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Events::SelectionContextChange>(BIND_EVENT(Hierarchy::SelectionContextChange));
		return dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT(Hierarchy::KeyPressEvent));
	}
	void Hierarchy::OnWindowRender()
	{
		Layout::SameLine(0, 5);
		ImGui::SetNextItemWidth(Layout::GetMaxWidth() - 20);
		Input::InputField(searchValue, "Search");
		Layout::SameLine();

		Style::SetButtonColors("#DB3721", "#C3311D", "#A02818");
		Style::SelectFont(1);

		if (Input::Button("X", { 20, 20 })) {
			searchValue = "";
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		Ref<World> world = ECS::WorldCommand::GetCurrentWorld();

		bool open = Layout::Treenode(world->GetName().c_str(), Style::GetTreeNodeDefaultFlags());

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
			EditorView::GetInstance().SetLayerActive<WorldEnvironmentData>(true);
		}
		if (open) {
			world->GetWorldRegistry().each([&](auto entityID) {
				Entity entity { entityID, world.Raw() };
				DrawEntity(entity);
				});
			ImGui::TreePop();
		}

		ContextMenus::HierarchyEntityMenu(world, [](Entity entity) {
			Events::SelectionContextChange e(entity);
			EditorView::GetInstance().OnEvent(e);
			});

		if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(0)) {
			Events::SelectionContextChange e({});
			EditorView::GetInstance().OnEvent(e);
		}
	}
	bool Hierarchy::SelectionContextChange(Events::SelectionContextChange& e)
	{
		selectionContext = e.GetEntity();
		return false;
	}
	void Hierarchy::DrawEntity(Entity entity) {
		ImGuiTreeNodeFlags flags = ((selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow |
			ImGuiTreeNodeFlags_SpanAvailWidth;

		auto& tag = entity.GetComponent<TagComponent>();

		if (searchValue != "" && strstr(tag.m_Tag.c_str(), searchValue.c_str()) == 0) {
			return;
		}

		bool opened = Layout::Treenode((void*)(uint64_t)(uint32_t)entity, tag.m_Tag.c_str(), flags, [&]() {});

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem()) {
			Layout::MenuItem("Delete entity", [&entity]() {
				entity.GetWorld().GetWorldRegistry().destroy(entity);
				});
			ImGui::EndPopup();
		}

		bool drag = DragDropUtils::DragSource("Hazard.Entity", entity.GetTag().m_Tag, std::to_string((uint32_t)entity + 1));

		if (ImGui::IsMouseReleased(0) && ImGui::IsItemHovered()) {
			if (entityDeleted) {
				Events::SelectionContextChange e({});
				EditorView::GetInstance().OnEvent(e);
				return;
			}
			else {
				Events::SelectionContextChange e(entity);
				EditorView::GetInstance().OnEvent(e);
			}
		}

		if (opened) {
			Layout::Text(std::string("ID: " + std::to_string(entity)).c_str());
			Layout::Text(entity.IsVisible() ? "Visible" : "Not visible");
		}
	}
	bool Hierarchy::KeyPressEvent(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == Key::Delete && selectionContext) {
			selectionContext.GetWorld().DestroyEntity(selectionContext);
			return true;
		}
		if (e.GetKeyCode() == Key::D && selectionContext && Hazard::Input::IsKeyDown(Key::LeftControl)) {
			selectionContext.GetWorld().CreateEntity(selectionContext);
			return true;
		}
		return false;
	}
}
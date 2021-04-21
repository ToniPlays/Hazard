#pragma once

#include <hzreditor.h>
#include "Hierarchy.h"
#include "GUI/EditorView.h"

#include "GUI/Library/Input.h"
#include "GUI/Library/Style.h"
#include "GUI/Library/Layout/Layout.h"
#include "GUI/Library/Layout/ContextMenus.h"

using namespace WindowLayout;

namespace WindowElement {

	Hierarchy::Hierarchy() : EditorWindow(ICON_FK_ALIGN_LEFT " Hierarchy")
	{

	}
	Hierarchy::~Hierarchy()
	{

	}
	void Hierarchy::Init()
	{

	}
	bool Hierarchy::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Events::SelectionContextChange>(BIND_EVENT(Hierarchy::SelectionContextChange));
		return dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT(Hierarchy::KeyPressEvent));
		
	}
	void Hierarchy::OnWindowRender()
	{
		Layout::SameLine(0, 5);
		Layout::Text("Find");
		Layout::SameLine(45);
		ImGui::SetNextItemWidth(Layout::GetMaxWidth() - 20);
		Input::InputField(searchValue);
		Layout::SameLine();

		Appereance::Style::SetButtonColors("#DB3721", "#C3311D", "#A02818");
		Appereance::Style::SelectFont(1);

		if (Input::Button("X", { 20, 20 })) {
			searchValue = "";
		}
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		World& world = ECS::SceneCommand::GetCurrentWorld();
		Layout::Treenode(world.GetName().c_str(), Appereance::Style::GetTreeNodeDefaultFlags(), [&world, this]() {
			world.GetWorldRegistry().each([&](auto entityID) {
				Entity entity{ entityID, &world };
				DrawEntity(entity);
			});
		});

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

		if (ImGui::IsItemClicked()) {
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
			Layout::Text(entity.IsVisible() ? "Visible" : "Not visible");
		}
	}
	bool Hierarchy::KeyPressEvent(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == Key::Delete && selectionContext) {
			selectionContext.GetWorld().DestroyEntity(selectionContext);
			return true;
		}
		return false;
	}
}
#pragma once

#include <hzreditor.h>
#include "Hierarchy.h"
#include "GUI/EditorView.h"

#include "GUI/Library/Layout.h"
#include "GUI/Library/Style.h"

using namespace WindowLayout;

namespace WindowElement {

	Hierarchy::Hierarchy() : EditorWindow("Hierarchy")
	{

	}
	Hierarchy::~Hierarchy()
	{

	}
	void Hierarchy::Init()
	{
		bool found = false;
		handler = &Application::GetModule<SceneHandler>(found);
		SetActive(found);
	}
	bool Hierarchy::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Events::SelectionContextChange>(BIND_EVENT(Hierarchy::SelectionContextChange));
		return false;
	}
	void Hierarchy::OnWindowRender()
	{
		Scene& scene = handler->GetCurrentScene();
		Layout::Treenode(scene.GetSceneFile().c_str(), Style::Style::GetTreeNodeDefaultFlags(), [&scene, this]() {
			scene.GetSceneRegistry().each([&](auto entityID) {
				Entity entity{ entityID, &scene };
				DrawEntity(entity);
			});
		});

		Layout::ContextMenu([&scene]() {
			Layout::MenuItem("Create entity", [&scene]() {
				scene.CreateEntity("Empty entity");
				
			});
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
		
		bool opened = Layout::Treenode((void*)(uint64_t)(uint32_t)entity, tag.tag.c_str(), flags, [&]() {
			
		});

		bool entityDeleted = false;

		if (ImGui::BeginPopupContextItem()) {
			Layout::MenuItem("Delete entity", [&entity]() {
				entity.GetScene().GetSceneRegistry().destroy(entity);
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
	}
}
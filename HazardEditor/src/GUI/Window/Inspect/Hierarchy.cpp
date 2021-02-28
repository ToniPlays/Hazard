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
		Layout::Treenode(scene.GetSceneFile(), Style::Style::GetTreeNodeDefaultFlags(), [&scene, this]() {
			scene.GetSceneRegistry().each([&](auto entityID) {
				Entity entity { entityID, &scene };
				DrawEntity(entity);
			});
		});

		Layout::ContextMenu([&scene]() {
			Layout::MenuItem("Create entity", [&scene]() {
				scene.CreateEntity("New entity");
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
			Layout::Text(std::string("Hello is me entity named " + tag.tag).c_str());
		});

		if (ImGui::IsItemClicked()) {
			Events::SelectionContextChange e(entity);
			EditorView::GetInstance().OnEvent(e);
		}
	}
}
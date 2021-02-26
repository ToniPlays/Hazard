#pragma once

#include <hzreditor.h>
#include "Hierarchy.h"
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
	void Hierarchy::OnWindowRender()
	{
		Scene& scene = handler->GetCurrentScene();
		Layout::Treenode(scene.GetSceneFile(), Style::Style::GetTreeNodeDefaultFlags(), [&scene, this]() {
			scene.GetSceneRegistry().each([&](auto entityID) {
				Entity entity { entityID, &scene };
				DrawEntity(entity);
			});
		});

		Layout::ContextMenu([]() {
			Layout::MenuItem("Create entity", []() {
				HZR_INFO("Create new entity TODO");
			});
		});
	}
	void Hierarchy::DrawEntity(Entity entity) {
		ImGuiTreeNodeFlags flags = ((selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow |
			ImGuiTreeNodeFlags_SpanAvailWidth;

		bool opened = Layout::Treenode(entity.GetComponent<TagComponent>().tag.c_str(), flags, [&entity]() {
			Layout::Text("Hello is me entity");
		});
		if (ImGui::IsItemClicked()) {
			SetSelectionContext(entity);
		}

	}
}
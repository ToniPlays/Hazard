#pragma once

#include <hzreditor.h>
#include "EngineAssets.h"
#include "GUI/Library/Layout/Layout.h"
#include "GUI/Library/Style.h"
#include "GUI/Library/Input.h"

using namespace WindowLayout;

namespace WindowElement {
	EngineAssets::EngineAssets() : EditorWindow(ICON_FK_EXCLAMATION_TRIANGLE" Engine assets") 
	{
		SetActive(false);
	}
	EngineAssets::~EngineAssets() {}
	void EngineAssets::Init() {}

	void EngineAssets::OnWindowRender()
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanFullWidth;

		Layout::Treenode("Shaders", flags, []() {
			Layout::Table(2, false);
			Layout::SetColumnWidth(50);
			Layout::EndTable();
		});

		Layout::Treenode("Textures", flags, []() {
			
		});
		Layout::Treenode("Meshes", flags, []() {
			for (Mesh* mesh : MeshFactory::GetLoadedMeshes()) {
				Layout::Text(mesh->GetFile().c_str());
			}
		});
		Layout::NextLine(25);
	}
}
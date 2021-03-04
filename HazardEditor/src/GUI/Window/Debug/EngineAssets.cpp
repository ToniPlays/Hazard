#pragma once

#include <hzreditor.h>
#include "EngineAssets.h"
#include "GUI/Library/Layout.h"
#include "GUI/Library/Style.h"

using namespace WindowLayout;

namespace WindowElement {
	EngineAssets::EngineAssets() : EditorWindow("Engine assets")
	{

	}
	EngineAssets::~EngineAssets()
	{

	}
	void EngineAssets::Init()
	{

	}
	void EngineAssets::OnWindowRender()
	{
		Layout::Treenode("Loaded textures", Style::Style::GetTreeNodeDefaultFlags(), []() {
			auto textures = Hazard::Rendering::RenderUtils::GetTextures();
			Layout::Table(2, false);


			for (Texture* t : textures) {
				Layout::SetColumnWidth(110);
				ImGui::Image((void*)t->GetID(), { 75, 75 }, { 0, 0 }, {1, 1});
				Layout::TableNext();
				Layout::Text(t->GetFile());
				std::stringstream ss;
				ss << "Texture ID: " << t->GetID() << "\nSize: " << t->GetWidth() << "x" << t->GetHeight();
				Layout::Text(ss.str().c_str());
				Layout::TableNext();
			}
			Layout::EndTable();
		});
	}
}
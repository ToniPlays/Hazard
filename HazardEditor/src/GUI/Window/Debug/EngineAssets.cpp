#pragma once

#include <hzreditor.h>
#include "EngineAssets.h"
#include "GUI/Library/Layout/Layout.h"
#include "GUI/Library/Style.h"
#include "GUI/Library/Input.h"

using namespace WindowLayout;

namespace WindowElement {
	EngineAssets::EngineAssets() : EditorWindow(ICON_FK_EXCLAMATION_TRIANGLE" Engine assets") {}
	EngineAssets::~EngineAssets() {}
	void EngineAssets::Init() {}

	void EngineAssets::OnWindowRender()
	{
		using namespace Appereance;
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanFullWidth;

		Layout::Treenode("Shaders", flags, []() {

		});

		Layout::Treenode("Textures", flags, []() {
			Layout::Table(2, false);
			Layout::EndTable();

			static std::string texture;
			if (Input::Button("Load texture")) {
				Rendering::RenderUtils::Create<Texture2D>(texture.c_str());
			}
			Layout::SameLine(125);
			Input::InputField(texture);
		});
		Layout::Treenode("Meshes", flags, []() {
			for (Mesh* mesh : MeshFactory::GetLoadedMeshes()) {
				Layout::Text(mesh->GetFile().c_str());
			}
		});
		Layout::NextLine(25);
	}
}
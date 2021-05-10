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
			for (Shader* shader : RenderUtils::GetShaders()) {
				Layout::Text(shader->GetFile());
			}
		});

		Layout::Treenode("Textures", flags, []() {
			auto textures = Hazard::Rendering::RenderUtils::GetTextures();
			Layout::Table(2, false);

			for (Texture* t : textures) {
				
				bool changed = Input::TextureSlot(t, [&]() {
					
				});
				Layout::TableNext();
			}
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
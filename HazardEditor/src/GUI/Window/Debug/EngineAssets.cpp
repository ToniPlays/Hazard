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
			Layout::Table(2, false);
			Layout::SetColumnWidth(50);
			for (RefCount* ref : RenderUtils::GetAssetsOf(AssetType::ShaderAsset)) {
				Shader* shader = (Shader*)ref;

				Layout::Text(std::to_string(ref->GetRefCount()).c_str());
				Layout::TableNext();
				ImGui::Text(shader->GetInfo().shaderName.c_str());
				Layout::TableNext();
			}
			Layout::EndTable();
		});

		Layout::Treenode("Textures", flags, []() {
			for (RefCount* ref : RenderUtils::GetAssetsOf(AssetType::TextureAsset)) {
				Texture* texture = (Texture*)ref;
				Input::TextureSlot(texture, [&]() {});
			}
			
		});
		Layout::Treenode("Meshes", flags, []() {
			for (Mesh* mesh : MeshFactory::GetLoadedMeshes()) {
				Layout::Text(mesh->GetFile().c_str());
			}
		});
		Layout::NextLine(25);
	}
}
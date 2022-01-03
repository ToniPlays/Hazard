
#include "EngineAssets.h"
#include "Library/Layout/Layout.h"
#include "Library/Style.h"
#include "Library/Input.h"

using namespace WindowLayout;

namespace WindowElement {
	EngineAssets::EngineAssets() : EditorWindow(ICON_FK_EXCLAMATION_TRIANGLE" Engine assets") 
	{
		SetActive(false);
	}
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
			/*for (Mesh* mesh : MeshFactory::GetLoadedMeshes()) {
				Layout::Text(mesh->GetFile().c_str());
			}*/
		});
		Layout::NextLine(25);
	}
}

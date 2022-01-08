
#include "EngineAssets.h"
#include "Library/Layout/Layout.h"
#include "Library/Style.h"
#include "Library/Input.h"
#include "Hazard/Assets/AssetManager.h"

using namespace WindowLayout;

namespace WindowElement {
	EngineAssets::EngineAssets() : EditorWindow(ICON_FK_EXCLAMATION_TRIANGLE" Engine assets") 
	{
		SetActive(true);
	}
	void EngineAssets::OnWindowRender()
	{
		if (ImGui::Button("Refresh")) {
			AssetManager::RemoveUnreferencedResources();
		}
		return;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Framed;
		for (auto& [path, meta] : Hazard::AssetManager::GetMetadataRegistry()) 
		{
			AssetMetadata& data = meta;
			Layout::Treenode(path.string().c_str(), flags, [&]() {
				ImGui::Text("Loaded: %s", data.IsLoaded ? "True" : "False");
			});
		}
		ImGui::Separator();

		for (auto& [handle, resource] : Hazard::AssetManager::GetRuntimeResources())
		{
			AssetHandle resourceHandle = handle;
			Ref<RuntimeResource> res = resource;
			ImGui::PushID(std::to_string(handle).c_str());
			Layout::Treenode(Utils::ResourceTypeToString(resource->GetType()), flags, [&]() {
				ImGui::Text("Handle: %s", std::to_string(resourceHandle).c_str());
				ImGui::Text("References: %s", std::to_string(res->GetRefCount() - 2).c_str());
				});
			ImGui::PopID();
		}
	}
}

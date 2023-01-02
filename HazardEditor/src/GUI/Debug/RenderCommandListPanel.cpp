
#include "RenderCommandListPanel.h"
#include "Hazard/Rendering/RenderEngine.h"
#include "Hazard/Rendering/Renderers/WorldRenderer.h"

using namespace Hazard;

namespace UI
{
	void RenderCommandListPanel::OnPanelRender()
	{
		HZR_PROFILE_FUNCTION();
		RenderEngine& engine = Application::GetModule<RenderEngine>();
		auto& drawList = engine.GetDrawLists();

		for (auto& list : drawList)
		{
			Ref<WorldRenderer> renderer = list.WorldRenderer;
			if (!renderer->IsValid()) continue;

			ImUI::Treenode(renderer->GetTargetWorld()->GetName().c_str(), ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen, [&]() {
				const char* elements[] = { "Type", "Value" };

				ImVec2 size = ImGui::GetContentRegionAvail();
				size.y = 150.0f;

				ImUI::Table("WorldRenderer", elements, 2, size, [&]() {

					ImGui::TableNextRow(0);

					ImGui::TableNextColumn();
					ImGui::Text("Camera count");
					ImGui::TableNextColumn();
					ImGui::Text("%s", std::to_string(renderer->GetCameraData().size()).c_str());

					ImGui::TableNextColumn();
					ImGui::Text("Environment count");
					ImGui::TableNextColumn();
					ImGui::Text("%s", std::to_string(list.Environment.size()).c_str());

					ImGui::TableNextColumn();
					ImGui::Text("Quad count");
					ImGui::TableNextColumn();
					ImGui::Text("%s", std::to_string(list.Stats.QuadCount).c_str());

					ImGui::TableNextColumn();
					ImGui::Text("Mesh count");
					ImGui::TableNextColumn();
					ImGui::Text("%s", std::to_string(list.Stats.MeshCount).c_str());

					ImGui::TableNextColumn();
					ImGui::Text("Vertices");
					ImGui::TableNextColumn();
					ImGui::Text("%s", std::to_string(list.Stats.Vertices).c_str());

					ImGui::TableNextColumn();
					ImGui::Text("Indices");
					ImGui::TableNextColumn();
					ImGui::Text("%s", std::to_string(list.Stats.Indices).c_str());

					ImGui::TableNextColumn();
					ImGui::Text("Draw calls");
					ImGui::TableNextColumn();
					ImGui::Text("%s", std::to_string(list.Stats.DrawCalls).c_str());

					});
				DrawDetailedInfo(list);
				});
		}
	}

	void RenderCommandListPanel::DrawDetailedInfo(RendererDrawList& drawList)
	{
		ImUI::Treenode("Cameras", ImGuiTreeNodeFlags_Framed, [&]() {

			for (auto& data : drawList.WorldRenderer->GetCameraData()) {

				const glm::vec3& pos = glm::inverse(data.View)[3];
				ImGui::TableNextColumn();
				ImGui::Text("Position");
				ImGui::TableNextColumn();
				ImGui::Text("[%.2f, %.2f, %.2f]", pos.x, pos.y, pos.z);
			}
			});
		ImUI::Treenode("Shadow pass", ImGuiTreeNodeFlags_Framed, [&]() {});
		ImUI::Treenode("Geometry pass", ImGuiTreeNodeFlags_Framed, [&]() {
			for (auto& [pipeline, list] : drawList.MeshList) {
				auto& meshDrawList = list;
				ImUI::Treenode(pipeline->GetSpecifications().DebugName.c_str(), 0, [&]() {
					ImGui::Columns(2, 0, false);

					/*for (auto& mesh : meshDrawList) {
						ImGui::Text("Index count");
						ImGui::NextColumn();
						ImGui::Text("%s", std::to_string(mesh.).c_str());
						ImGui::NextColumn();
					}*/
					ImGui::Columns();
					});
			}
			});
		ImUI::Treenode("Composite pass", ImGuiTreeNodeFlags_Framed, [&]() {});
	}
}


#include "RenderCommandListPanel.h"
#include "Hazard/Rendering/RenderEngine.h"
#include "Hazard/Rendering/Renderers/WorldRenderer.h"
#include "Hazard/ImGUI/UIElements/Treenode.h"

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
			if (!renderer) continue;

			if (!renderer->IsValid()) continue;

			ImUI::Treenode commandList("WorldCommandList", true);
			commandList.DefaultOpen();
			commandList.Content([&]() {
				ImVec2 size = ImGui::GetContentRegionAvail();
				size.y = 150.0f;

				ImUI::Table("WorldRenderer", { "Type", "Value" }, size, [&]() {

					ImGui::TableNextRow(0);

					ImGui::TableNextColumn();
					ImGui::Text("Camera count");
					ImGui::TableNextColumn();
					ImGui::Text("%s", std::to_string(renderer->GetCameraData().size()).c_str());

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
			commandList.Render();
		}
	}

	void RenderCommandListPanel::DrawDetailedInfo(RendererDrawList& drawList)
	{
		ImUI::Treenode cameras("Cameras", true);
		cameras.DefaultOpen();
		cameras.Content([&]() {
			for (auto& data : drawList.WorldRenderer->GetCameraData())
			{
				const glm::vec3& pos = glm::inverse(data.View)[3];
				ImGui::TableNextColumn();
				ImGui::Text("Position");
				ImGui::TableNextColumn();
				ImGui::Text("[%.2f, %.2f, %.2f]", pos.x, pos.y, pos.z);
			}
		});
	}
}


#include "RenderCommandListPanel.h"
#include "Hazard/Rendering/RenderEngine.h"
#include "Hazard/Rendering/Renderers/WorldRenderer.h"
#include "Hazard/ImGUI/UIElements/Treenode.h"
#include "Hazard/ImGUI/UIElements/Table.h"

using namespace Hazard;

namespace UI
{
	void RenderCommandListPanel::OnPanelRender()
	{
		HZR_PROFILE_FUNCTION();
		RenderEngine& engine = Application::Get().GetModule<RenderEngine>();
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

				struct Content 
				{
					std::string Key;
					std::string Value;
				};

				ImUI::Table<Content> table("WorldRenderer", size);
				table.SetColumns({ "Type", "Value" });
				table.RowHeight(24.0f);
				table.RowContent([](uint32_t, const Content& item) {
					ImGui::Text("%s", item.Key.c_str());
					ImGui::TableNextColumn();
					ImGui::Text("%s", item.Value.c_str());
				});

				table.AddRow({ "Camera", std::to_string(renderer->GetCameraData().size()) });
				table.AddRow({ "Quads", std::to_string(list.Stats.QuadCount) });
				table.AddRow({ "Mesh", std::to_string(list.Stats.MeshCount) });
				table.AddRow({ "Vertices", std::to_string(list.Stats.Vertices) });
				table.AddRow({ "Indices", std::to_string(list.Stats.Indices) });
				table.AddRow({ "Draws", std::to_string(list.Stats.DrawCalls) });

				table.Render();
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

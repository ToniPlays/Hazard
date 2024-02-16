
#include "RendererDebugPanel.h"
#include "Hazard/Rendering/RenderEngine.h"
#include "Hazard/ImGUI/UIElements/Treenode.h"
#include "Hazard/ImGUI/UIElements/Table.h"

namespace UI
{
	using namespace Hazard;
	void RendererDebugPanel::OnPanelRender()
	{
		HZR_PROFILE_FUNCTION();

		auto& drawlists = Application::Get().GetModule<RenderEngine>().GetDrawLists();

		for (auto& drawlist : drawlists)
		{
			Ref<World> target = drawlist.WorldRenderer->GetTargetWorld();
			ImUI::Treenode listNode(fmt::format("World: {0}", target->GetHandle()), true);
			listNode.Content([this, drawlist]() {
				DrawWorldDrawList(drawlist);
			});
			listNode.Render();
		}
	}
	void RendererDebugPanel::DrawWorldDrawList(const Hazard::RendererDrawList& list)
	{
		ImUI::Treenode listNode("Cameras", true);
		listNode.Content([this, list]() {

			ImUI::Table<WorldCameraData> table("Cameras", { ImGui::GetContentRegionAvail().x, 0 });
			table.SetColumns({ "Camera", "Position" });
			table.RowHeight(24.0f);
			table.RowContent([](uint32_t i, WorldCameraData& data) {
				ImGui::PushID(i);

				ImUI::ShiftX(4.0f);
				ImGui::Text("Camera %u", i);

				ImGui::TableNextColumn();
				ImUI::ShiftX(4.0f);
				ImGui::Text("%.2f, %.2f, %.2f", data.Position.x, data.Position.y, data.Position.z);

				ImGui::PopID();
			});

			for (auto& camera : list.WorldRenderer->GetCameraData())
				table.AddRow(camera);

			table.Render();

		});
		listNode.Render();

		RenderGeometryPass(list.GeometryPass);
	}
	void RendererDebugPanel::RenderGeometryPass(const std::map<Hazard::MeshKey, Hazard::MeshData>& meshes)
	{
		ImUI::Treenode listNode("Geometry pass", true);
		listNode.Content([this, meshes]() {

			ImUI::Table<MeshData> table("Instances", { ImGui::GetContentRegionAvail().x, 0 });
			table.SetColumns({ "Vertex buffer", "Material", "Count", "Instance count", "Transform offset" });
			table.RowHeight(24.0f);
			table.RowContent([](uint32_t i, MeshData& data) {
				ImGui::PushID(i);

				ImUI::ShiftX(4.0f);
				ImGui::Text(data.VertexBuffer->GetDebugName().c_str());

				ImGui::TableNextColumn();
				ImUI::ShiftX(4.0f);
				ImGui::Text("%s", data.Material->GetPipeline()->GetSpecifications().DebugName.c_str());

				ImGui::TableNextColumn();
				ImUI::ShiftX(4.0f);
				ImGui::Text("%u", data.Count);

				ImGui::TableNextColumn();
				ImUI::ShiftX(4.0f);
				ImGui::Text("%u", data.Transforms.size());

				ImGui::TableNextColumn();
				ImUI::ShiftX(4.0f);
				ImGui::Text("%u", data.TransformOffset);

				ImGui::PopID();
			});

			for (auto& [key, data] : meshes)
				table.AddRow(data);

			table.Render();

		});
		listNode.Render();
	}
}
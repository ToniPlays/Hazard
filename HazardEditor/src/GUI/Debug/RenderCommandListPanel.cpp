
#include "RenderCommandListPanel.h"
#include "Hazard/Rendering/RenderEngine.h"

using namespace Hazard;

namespace UI
{
	void RenderCommandListPanel::OnPanelRender()
	{
		RenderEngine& engine = Application::GetModule<RenderEngine>();
		RendererDrawList& drawList = engine.GetDrawList();

		UI::Treenode("Rendering cameras", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed, [&]() {
			for (auto& camera : drawList.RenderingCameras) {
				UI::Treenode(camera.DebugName.c_str(), ImGuiTreeNodeFlags_Framed, [camera]() {
					ImGui::Columns(2, 0, false);
					ImGui::SetColumnWidth(0, 125.0f);

					ImGui::Text("Position");
					ImGui::NextColumn();
					ImGui::Text("%.3f, %.3f, %.3f", camera.Position.x, camera.Position.y, camera.Position.z);
					ImGui::NextColumn();

					ImGui::Text("ViewProjection");
					ImGui::NextColumn();
					ImGui::NextColumn();

					ImGui::Text("Geometry flags");
					ImGui::NextColumn();
					ImGui::NextColumn();

					ImGui::Columns();
					});
			}
			});
	}
}
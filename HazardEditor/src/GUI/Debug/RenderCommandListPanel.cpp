
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
		UI::Treenode("Lights", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed, [&]() {
			for (auto& light : drawList.LightSource) {
				Group(&light, [light]() {
					UI::Treenode("Light", ImGuiTreeNodeFlags_Framed, [light]() {
						ImGui::Columns(2, 0, false);
						ImGui::SetColumnWidth(0, 125.0f);
						ImGui::Text("Intensity");
						ImGui::NextColumn();
						ImGui::Text("%.3f", light.Intensity);
						ImGui::NextColumn();
						ImGui::Text("Color");
						ImGui::NextColumn();
						ImGui::Text("%.3f, %.3f, %.3f", light.Color.r, light.Color.g, light.Color.b);
						ImGui::NextColumn();

						ImGui::Columns();
						});
					});
			}
			});
		UI::Treenode("Meshes", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed, [&]() 
			{
			for (auto& [pipeline, meshList] : drawList.Meshes) {
				auto& meshes = meshList;
				Group(pipeline, [p = pipeline, meshes]() {

					UI::Treenode(p->GetSpecifications().DebugName.c_str(), ImGuiTreeNodeFlags_Framed, [&]()
						{
						for (auto mesh : meshes) {
							UI::Treenode("Mesh", ImGuiTreeNodeFlags_Framed, [&]()
								{
									uint32_t vertexCount = mesh.VertexBuffer->GetSize();
									uint32_t indexCount = mesh.Count;
									const glm::mat4& t = mesh.Transform;

									ImGui::Columns(2, 0, false);
									ImGui::SetColumnWidth(0, 125.0f);
									ImGui::Text("Vertex count");
									ImGui::NextColumn();
									ImGui::Text("%i", vertexCount);
									ImGui::NextColumn();

									ImGui::Text("Index count");
									ImGui::NextColumn();
									ImGui::Text("%i", indexCount);
									ImGui::NextColumn();

									glm::vec3 position;
									glm::vec3 rotation;
									glm::vec3 scale;

									Math::DecomposeTransform(t, position, rotation, scale);

									ImGui::Text("Position");
									ImGui::NextColumn();
									ImGui::Text("%.2f, %.2f, %.2f", position.x, position.y, position.z);
									ImGui::NextColumn();

									ImGui::Columns();
								});
						}
						});
					});
			}
			});
	}
}
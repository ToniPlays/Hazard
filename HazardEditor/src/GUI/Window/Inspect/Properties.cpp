
#include "Properties.h"
#include "Library/Input.h"
#include "Library/Layout/Layout.h"
#include "Library/Style.h"
#include "Library/ComponentDraw.h"
#include "Library/Layout/ContextMenus.h"
#include "Core/SceneRuntimeHandler.h"

using namespace WindowLayout;

namespace WindowElement {

	Properties::Properties() : EditorWindow(ICON_FK_WRENCH " Properties")
	{

	}
	void Properties::Init()
	{
		Runtime::SceneRuntimeHandler::AddRuntimeCallback([&](bool runtime) {
			m_SelectionContext = Application::GetModule<WorldHandler>().GetCurrentWorld()->GetEntity(m_SelectionContext);
			});
	}
	void Properties::OnUpdate()
	{
		HZR_PROFILE_FUNCTION();
		if (!m_SelectionContext.IsValid()) return;

		auto& tc = m_SelectionContext.GetComponent<TransformComponent>();

		if (m_SelectionContext.HasComponent<CameraComponent>())
		{
			auto& comp = m_SelectionContext.GetComponent<CameraComponent>();

			//std::vector<glm::vec3> points = Math::GetProjectionBounds(tc.GetOrientation(), tc.GetTransformMat4(), comp.GetFov(),
			//	comp.GetClipping().x, comp.GetClipping().y, comp.GetAspectRatio());

			for (uint32_t i = 0; i < 4; i++)
			{
				//Hazard::Rendering::RenderCommand::DrawLine(points[i] + tc.Translation, points[(i + 1) % 4] + tc.Translation, Color::White);
			}
			for (uint32_t i = 0; i < 4; i++)
			{
				//Hazard::Rendering::RenderCommand::DrawLine(points[i] + tc.Translation, points[i + 4] + tc.Translation, Color::White);
			}
			for (uint32_t i = 0; i < 4; i++)
			{
				//Hazard::Rendering::RenderCommand::DrawLine(points[i + 4] + tc.Translation, points[((i + 1) % 4) + 4] + tc.Translation, Color::White);
			}
		}
		if (m_SelectionContext.HasComponent<BoxCollider2DComponent>())
		{
			auto& bc2d = m_SelectionContext.GetComponent<BoxCollider2DComponent>();
			glm::vec3 translation = tc.Translation + glm::vec3(bc2d.Offset, 0.005f);
			glm::vec3 scale = tc.Scale * glm::vec3(bc2d.Size * 2.0f, 1.0f);

			//Hazard::Rendering::RenderCommand::DrawRectangle(Math::ToTransformMatrix(translation, { 0.0f, 0.0f, tc.Rotation.z }, scale), Color::Blue);
		}
		if (m_SelectionContext.HasComponent<CircleCollider2DComponent>())
		{
			auto& cc2d = m_SelectionContext.GetComponent<CircleCollider2DComponent>();
			glm::vec3 translation = tc.Translation + glm::vec3(cc2d.Offset, 0.005f);
			glm::vec3 scale = tc.Scale.x * glm::vec3(cc2d.Radius * 2.0f, cc2d.Radius * 2.0f, 1.0f);

			//Hazard::Rendering::RenderCommand::DrawCircle(Math::ToTransformMatrix(translation, { 0.0f, 0.0f, 0.0f }, scale), cc2d.Radius, 0.03f, Color::Blue);
		}
	}
	void Properties::OnWindowRender()
	{
		if (!m_SelectionContext.IsValid()) return;

		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);

		if (ImGui::Button(ICON_FK_CUBE, { 35, 35 }))
		{

		}
		Layout::Tooltip("Gizmo maybe?");
		Layout::SameLine(0, 2);

		if (ImGui::Button(m_SelectionContext.IsVisible() ? ICON_FK_EYE : ICON_FK_EYE_SLASH, { 35, 35 })) {
			m_SelectionContext.SetVisible(!m_SelectionContext.IsVisible());
		}
		Layout::Tooltip("Toggle visibility");
		auto& c = m_SelectionContext.GetComponent<TagComponent>();
		ImVec2 textWidth = ImGui::CalcTextSize(c.Tag.c_str());

		ImGui::PopStyleVar();
		ImGui::SameLine();

		Style::SelectFont(1);

		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - (textWidth.x / 2));
		ImGui::SetCursorPosY(35 / 2 + (textWidth.y / 2) + 10);
		Layout::Text(c.Tag.c_str());
		ImGui::PopFont();

		if (m_SelectionContext.HasComponent<MeshComponent>()) {
			auto& m = m_SelectionContext.GetComponent<MeshComponent>();
			/*if (m.m_Mesh.Raw() != nullptr) {
				Rendering::VertexArray& arr = m.m_Mesh->GetVertexArray();
				Rendering::RenderCommand::DrawGizmo(&arr, arr.GetIndexBuffer()->GetInfo().count);
			}*/
		}

		DrawComponent<TagComponent>(ICON_FK_TAG " Tag", m_SelectionContext);
		DrawComponent<TransformComponent>(ICON_FK_ARROWS " Transform", m_SelectionContext);
		DrawComponent<CameraComponent>(ICON_FK_CAMERA " Camera", m_SelectionContext);

		DrawComponent<ScriptComponent>(ICON_FK_SLACK " Script", m_SelectionContext);
		DrawComponent<VisualScriptComponent>(ICON_FK_SITEMAP " Visual script", m_SelectionContext);

		DrawComponent<SkyLightComponent>(ICON_FK_CLOUD " Sky light", m_SelectionContext);
		DrawComponent<DirectionalLightComponent>(ICON_FK_SUN_O " Directional light", m_SelectionContext);
		DrawComponent<PointLightComponent>(ICON_FK_LIGHTBULB_O " Point light", m_SelectionContext);

		DrawComponent<MeshComponent>(ICON_FK_CUBE " Mesh", m_SelectionContext);
		DrawComponent<SpriteRendererComponent>(ICON_FK_PICTURE_O" Sprite", m_SelectionContext);

		DrawComponent<AudioSourceComponent>(ICON_FK_VOLUME_UP" Audio Source", m_SelectionContext);

		DrawComponent<Rigidbody2DComponent>(ICON_FK_APPLE" RigidBody2D", m_SelectionContext);
		DrawComponent<BoxCollider2DComponent>(ICON_FK_APPLE" BoxCollider2D", m_SelectionContext);
		DrawComponent<CircleCollider2DComponent>(ICON_FK_APPLE" CircleCollider2D", m_SelectionContext);
		DrawComponent<BatchComponent>("Batch", m_SelectionContext);

		ContextMenus::PropertiesContextMenu(m_SelectionContext);
	}
	bool Properties::OnEvent(Event& e)
	{
		//EventDispatcher dispatcher(e);
		//dispatcher.Dispatch<Events::SelectionContextChange>(BIND_EVENT(Properties::SelectionContextChange));
		return false;
	}
	bool Properties::SelectionContextChange(Events::SelectionContextChange& e)
	{
		if (!IsLocked())
			m_SelectionContext = e.GetEntity();
		return false;
	}
}

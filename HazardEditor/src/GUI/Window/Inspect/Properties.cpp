#pragma once


#include "Properties.h"
#include "Library/Input.h"
#include "Library/Layout/Layout.h"
#include "Library/Style.h"
#include "Library/ComponentDraw.h"
#include "Library/Layout/ContextMenus.h"

using namespace WindowLayout;

namespace WindowElement {

	Properties::Properties() : EditorWindow(ICON_FK_WRENCH " Properties")
	{
	}
	void Properties::OnWindowRender()
	{
		if (!selectionContext.IsValid()) return;

		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0);

		if(ImGui::Button(ICON_FK_CUBE, { 35, 35 })) 
		{
			
		}
		Layout::Tooltip("Gizmo maybe?");
		Layout::SameLine(0, 2);

		if (ImGui::Button(selectionContext.IsVisible() ? ICON_FK_EYE : ICON_FK_EYE_SLASH, { 35, 35 })) {
			selectionContext.SetVisible(!selectionContext.IsVisible());
		}
		Layout::Tooltip("Toggle visibility");
		auto& c = selectionContext.GetComponent<TagComponent>();
		ImVec2 textWidth = ImGui::CalcTextSize(c.m_Tag.c_str());

		ImGui::PopStyleVar();
		ImGui::SameLine();

		Style::SelectFont(1);

		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - (textWidth.x / 2));
		ImGui::SetCursorPosY(35 / 2 + (textWidth.y / 2) + 10);
		Layout::Text(c.m_Tag.c_str());
		ImGui::PopFont();

		if (selectionContext.HasComponent<MeshComponent>()) {
			auto& m = selectionContext.GetComponent<MeshComponent>();
			/*if (m.m_Mesh.Raw() != nullptr) {
				Rendering::VertexArray& arr = m.m_Mesh->GetVertexArray();
				Rendering::RenderCommand::DrawGizmo(&arr, arr.GetIndexBuffer()->GetInfo().count);
			}*/
		}

		DrawComponent<TagComponent>(ICON_FK_TAG " Tag", selectionContext);
		DrawComponent<TransformComponent>(ICON_FK_ARROWS " Transform", selectionContext);
		DrawComponent<CameraComponent>(ICON_FK_CAMERA " Camera", selectionContext);

		DrawComponent<ScriptComponent>(ICON_FK_SLACK " Script", selectionContext);
		DrawComponent<VisualScriptComponent>(ICON_FK_SITEMAP " Visual script", selectionContext);

		DrawComponent<SkyLightComponent>(ICON_FK_CLOUD " Sky light", selectionContext);
		DrawComponent<DirectionalLightComponent>(ICON_FK_SUN_O " Directional light", selectionContext);
		DrawComponent<PointLightComponent>(ICON_FK_LIGHTBULB_O " Point light", selectionContext);

		DrawComponent<MeshComponent>(ICON_FK_CUBE " Mesh", selectionContext);
		DrawComponent<SpriteRendererComponent>(ICON_FK_PICTURE_O" Sprite", selectionContext);

		DrawComponent<AudioSourceComponent>(ICON_FK_VOLUME_UP" Audio Source", selectionContext);

		DrawComponent<Rigidbody2DComponent>(ICON_FK_APPLE" RigidBody2D", selectionContext);
		DrawComponent<BoxCollider2DComponent>(ICON_FK_APPLE" BoxCollider2D", selectionContext);
		DrawComponent<BatchComponent>("Batch", selectionContext);

		ContextMenus::PropertiesContextMenu(selectionContext);

		
	}
	bool Properties::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Events::SelectionContextChange>(BIND_EVENT(Properties::SelectionContextChange));
		return false;
	}
	bool Properties::SelectionContextChange(Events::SelectionContextChange& e)
	{
		if(!IsLocked()) 
			selectionContext = e.GetEntity();
		return false;
	}
}
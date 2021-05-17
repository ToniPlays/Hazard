#pragma once

#include <hzreditor.h>
#include "Properties.h"
#include "GUI/Library/Input.h"
#include "GUI/Library/Layout/Layout.h"
#include "GUI/Library/Style.h"
#include "GUI/Library/ComponentDraw.h"
#include "GUI/Library/Layout/ContextMenus.h"

using namespace WindowLayout;

namespace WindowElement {

	Properties::Properties() : EditorWindow(ICON_FK_WRENCH " Properties")
	{

	}
	Properties::~Properties()
	{

	}
	void Properties::OnWindowRender()
	{
		if (!selectionContext.IsValid()) return;

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

		ImGui::SameLine();

		Appereance::Style::SelectFont(1);

		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - (textWidth.x / 2));
		ImGui::SetCursorPosY(35 / 2 + (textWidth.y / 2));
		Layout::Text(c.m_Tag.c_str());
		ImGui::PopFont();

		DrawComponent<TagComponent>(ICON_FK_TAG " Tag", selectionContext);
		DrawComponent<TransformComponent>(ICON_FK_ARROWS " Transform", selectionContext);
		DrawComponent<CameraComponent>(ICON_FK_CAMERA " Camera", selectionContext);
		DrawComponent<SpriteRendererComponent>(ICON_FK_PICTURE_O" Sprite", selectionContext);

		DrawComponent<SkyLightComponent>(ICON_FK_CLOUD " Sky light", selectionContext);
		DrawComponent<DirectionalLightComponent>(ICON_FK_SUN_O " Directional light", selectionContext);
		DrawComponent<PointLightComponent>(ICON_FK_LIGHTBULB_O " Point light", selectionContext);
		DrawComponent<BatchComponent>("Batch", selectionContext);
		DrawComponent<ScriptComponent>(ICON_FK_SLACK " Script", selectionContext);
		DrawComponent<VisualScriptComponent>(ICON_FK_SITEMAP " Visual script", selectionContext);
		DrawComponent<MeshComponent>(ICON_FK_CUBE " Mesh", selectionContext);

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
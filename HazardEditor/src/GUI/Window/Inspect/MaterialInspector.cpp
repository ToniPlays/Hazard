#pragma once

#include <hzreditor.h>
#include "MaterialInspector.h"
#include "GUI/Library/Input.h"
#include "GUI/Library/Layout/Layout.h"
#include "GUI/Library/Style.h"
#include "GUI/Library/ComponentDraw.h"
#include "GUI/Library/Layout/ContextMenus.h"

using namespace WindowLayout;

namespace WindowElement {

	MaterialInspector::MaterialInspector() : EditorWindow(ICON_FK_WRENCH " Material Inspector")
	{

	}
	MaterialInspector::~MaterialInspector()
	{

	}
	void MaterialInspector::OnWindowRender()
	{
		if (!selectionContext.IsValid()) return;
		ImGui::Text(selectionContext.GetTag().m_Tag.c_str());
		if (!selectionContext.HasComponent<MeshComponent>()) return;

		MeshComponent meshComponent = selectionContext.GetComponent<MeshComponent>();
		Layout::Table(2, false);
		Layout::SetColumnWidth(75);
		Layout::Text("Shader");
		Layout::TableNext();
		Layout::MaxWidth();
		std::string name = meshComponent.m_Material->GetPipeline()->GetInfo().pipelineName;
		Input::InputField(name);
		Layout::EndTable();
		Layout::Treenode("Albedo", Style::GetTreeNodeDefaultFlags(), [&]() {

			Rendering::Texture2D* texture = meshComponent.m_Material->Get<Rendering::Texture2D*>("Material.AlbedoMap");

			Input::TextureSlot(texture, [&]() {
				Color color = meshComponent.m_Material->Get<Color>("Material.AlbedoColor");
				static bool open = false;
				if (Input::ColorPicker("Albedo tint", color, open))
				{
					meshComponent.m_Material->Set<Color>("Material.AlbedoColor", color);
				}
				}, [&]() {
					DragDropUtils::DragTarget("Texture2D", [&](const ImGuiPayload* payload) {
						const char* file = (const char*)payload->Data;
						Ref<Rendering::Texture2D> texture = Ref<Rendering::Texture2D>(Vault::Get<Rendering::Texture2D>(file));

						meshComponent.m_Material->Set("Material.AlbedoMap", texture);
					});
				});
			});

	}
	bool MaterialInspector::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Events::SelectionContextChange>(BIND_EVENT(MaterialInspector::SelectionContextChange));
		return false;
	}
	bool MaterialInspector::SelectionContextChange(Events::SelectionContextChange& e)
	{
		if (!IsLocked())
			selectionContext = e.GetEntity();
		return false;
	}
}
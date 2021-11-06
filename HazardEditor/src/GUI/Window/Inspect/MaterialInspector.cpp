#pragma once


#include "MaterialInspector.h"
#include "Library/Input.h"
#include "Library/Layout/Layout.h"
#include "Library/Style.h"
#include "Library/ComponentDraw.h"
#include "Library/Layout/ContextMenus.h"
#include "Core/EditorEvent.h"

using namespace WindowLayout;

namespace WindowElement {

	MaterialInspector::MaterialInspector() : EditorWindow(ICON_FK_WRENCH " Material Inspector")
	{
	}
	void MaterialInspector::OnWindowRender()
	{
		if (!selectionContext.IsValid()) return;

		ImGui::Text(selectionContext.GetTag().m_Tag.c_str());
		if (!selectionContext.HasComponent<MeshComponent>()) return;

		/*MeshComponent meshComponent = selectionContext.GetComponent<MeshComponent>();
		Layout::Table(2, false);
		Layout::SetColumnWidth(75);
		Layout::Text("Shader");
		Layout::TableNext();
		Layout::MaxWidth();

		std::string name = meshComponent.m_Material->GetPipeline()->GetInfo().pipelineName;
		Input::InputField(name);
		Layout::EndTable();
		float value = meshComponent.m_Material->Get<float>("Material.Metallic");
		if (Input::Slider("Metallic", value, 0.0f, 1.0f)) {
			meshComponent.m_Material->Set<float>("Material.Metallic", value);
		}
		value = meshComponent.m_Material->Get<float>("Material.Smoothness");
		if (Input::Slider("Smoothness", value, 0.0f, 1.0f)) {
			meshComponent.m_Material->Set<float>("Material.Smoothness", value);
		}

		Layout::Treenode("Albedo", Style::GetTreeNodeDefaultFlags(), [&]() {

			Rendering::Texture2D* texture = meshComponent.m_Material->Get<Rendering::Texture2D*>("Material.AlbedoMap");

			InputType type = Input::TextureSlot(texture, [&]() {
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
			if (type == InputType::Remove) {
				Ref<Rendering::Texture2D> texture = Ref<Rendering::Texture2D>(Vault::Get<Rendering::Texture2D>("White"));
				meshComponent.m_Material->Set("Material.AlbedoMap", texture);
			}
			});
			

		Layout::Treenode("Normal map", Style::GetTreeNodeDefaultFlags(), [&]() {

			Rendering::Texture2D* texture = meshComponent.m_Material->Get<Rendering::Texture2D*>("Material.NormalMap");

			InputType type = Input::TextureSlot(texture, [&]() {
				}, [&]() {
					DragDropUtils::DragTarget("Texture2D", [&](const ImGuiPayload* payload) {
						const char* file = (const char*)payload->Data;
						Ref<Rendering::Texture2D> texture = Ref<Rendering::Texture2D>(Vault::Get<Rendering::Texture2D>(file));

						meshComponent.m_Material->Set("Material.NormalMap", texture);
						});
				});
			if (type == InputType::Remove) {
				Ref<Rendering::Texture2D> texture = Ref<Rendering::Texture2D>(Vault::Get<Rendering::Texture2D>("DefaultNormalMap"));
				meshComponent.m_Material->Set("Material.NormalMap", texture);
			}
			});
			*/
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
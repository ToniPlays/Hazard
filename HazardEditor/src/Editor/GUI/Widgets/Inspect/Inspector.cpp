#pragma once

#pragma once
#include <hzreditor.h>
#include "Inspector.h"
#include "Editor/GUI/GUILibrary/Inputs.h"
#include "Editor/GUI/GUILibrary/VectorControl.h"


using namespace Hazard;

Inspector::Inspector() : Layer("Properties")
{

}

Inspector::~Inspector()
{
}

template<typename T, typename UIFuntion>
static void DrawComponent(const std::string& name, ECS::Entity entity, UIFuntion function) {
	const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth
		| ImGuiTreeNodeFlags_AllowItemOverlap;
	if (entity.HasComponent<T>()) {
		auto& component = entity.GetComponent<T>();
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();

		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, name.c_str());
		ImGui::PopStyleVar();

		ImGui::SameLine(contentRegionAvailable.x - lineHeight / 2.0f);
		if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight })) {
			ImGui::OpenPopup("Settings");
		}
		bool removeComponent = false;
		if (ImGui::BeginPopup("Settings")) {
			if (ImGui::MenuItem("Remove component")) {
				removeComponent = true;
			}
			ImGui::EndPopup();
		}
		if (open) {
			function(component);
			ImGui::TreePop();
		}
		if (removeComponent)
			entity.RemoveComponent<T>();
	}
}


static void DrawAllComponents(ECS::Entity entity) {
	
}

void Inspector::Render()
{
	if (!Panel::Begin(name, isLayerOpen)) return;

	if (context) {
		if (context.HasComponent<TagComponent>()) {
			auto& tag = context.GetComponent<TagComponent>().Tag;
			char buffer[256];

			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, tag.c_str(), sizeof(buffer));
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}
		DrawComponent<TransformComponent>("Transform component", context, [](auto& component) {
			VectorControl::DrawVec3("Position", component.Position);
			VectorControl::DrawVec3("Rotation", glm::degrees(component.Rotation));
			VectorControl::DrawVec3("Scale", component.Scale, 1.0f);
		});
		DrawComponent<CameraComponent>("Camera component", context, [](auto& component) {
		
			});
		DrawComponent<ScriptComponent>("Script component", context, [](auto& component) {
		
			});
		DrawComponent<MeshComponent>("Mesh component", context, [](auto& component) {
		
			});
		DrawComponent<MeshRendererComponent>("Mesh renderer component", context, [](auto& component) {
		
			});
		DrawComponent<SpriteRenderer>("Sprite renderer component", context, [](auto& component) {
		
			});

	}
	if (ImGui::BeginPopupContextWindow()) {
		if (ImGui::MenuItem("Camera")) {
			context.AddComponent<CameraComponent>();
		}
		if (ImGui::MenuItem("Script")) {
			context.AddComponent<ScriptComponent>();
		}
		if (ImGui::MenuItem("Mesh")) {
			context.AddComponent<MeshComponent>();
		}
		if (ImGui::MenuItem("Mesh renderer")) {
			context.AddComponent<MeshRendererComponent>();
		}
		if (ImGui::MenuItem("Sprite renderer")) {
			context.AddComponent<SpriteRenderer>();
		}
		ImGui::EndPopup();
	}
	Panel::End();
}





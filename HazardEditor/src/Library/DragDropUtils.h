#pragma once

#include <string>
#include "imgui_internal.h"

namespace WindowElement 
{
	class DragDropUtils {
	public:
		static bool DragSource(const char* type, const std::string& text, const void* handle);
		template<typename T>
		static void DragTarget(const char* type, T fn) 
		{
			if (ImGui::BeginDragDropTarget()) {
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(type);
				if (payload) {
					fn(payload);
				}
				ImGui::EndDragDropTarget();
			}
		}
		template<typename T>
		static void DragTargetCustom(const char* type, ImRect rect, ImGuiID id, T fn)
		{
			if (ImGui::BeginDragDropTargetCustom(rect, id)) {
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(type);
				if (payload) {
					fn(payload);
				}
				ImGui::EndDragDropTarget();
			}
		}
		static std::string TypeFromFile(const std::string& file);
	};
}
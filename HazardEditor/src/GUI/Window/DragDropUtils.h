#pragma once

namespace WindowElement 
{
	class DragDropUtils {
	public:
		static void DragSource(const char* type, const std::string& text, std::string file);
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
		static std::string TypeFromFile(const std::string& file);
	};
}
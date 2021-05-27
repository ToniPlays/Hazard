#pragma once

namespace WindowElement 
{
	class DragDropUtils {
	public:
		static void DragSource(const char* type, const std::string& text, void* data, uint32_t size);
		template<typename T>
		static void DragTarget(const char* type, T fn) 
		{
			ImGuiDragDropFlags flags = ImGuiDragDropFlags_SourceAllowNullID;

			if (ImGui::BeginDragDropTarget()) {
				const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(type, flags);
				if (payload) {
					fn(payload);
				}
				ImGui::EndDragDropTarget();
			}
		}
		static std::string TypeFromFile(const std::string& file);
	};
}
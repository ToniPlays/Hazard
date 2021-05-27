#pragma once
#include <hzreditor.h>
#include "DragDropUtils.h"

#include <imgui.h>

namespace WindowElement
{
	void DragDropUtils::DragSource(const char* type, const std::string& text, void* data, uint32_t size)
	{
		ImGuiDragDropFlags flags = ImGuiDragDropFlags_SourceAllowNullID;
		if (ImGui::BeginDragDropSource(flags)) 
		{
			ImGui::SetDragDropPayload(type, data, size);
			HZR_CORE_INFO(text);
			ImGui::Text(text.c_str());
			ImGui::EndDragDropSource();
		}
	}
	std::string DragDropUtils::TypeFromFile(const std::string& file)
	{
		std::string ext = File::GetFileExtension(file);

		if (ext == "jpeg") return "Texture2D";
		if (ext == "png") return "Texture2D";
		if (ext == "mp3") return "AudioClip";
		if (ext == "cs") return "Script";
		return "HAZARD_NULL";
	}
}
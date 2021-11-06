#pragma once

#include "Hazard.h"
#include "DragDropUtils.h"
#include <imgui.h>

using namespace Hazard;

namespace WindowElement
{
	bool DragDropUtils::DragSource(const char* type, const std::string& text, std::string file)
	{
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
			ImGui::SetDragDropPayload(type, (void*)file.c_str(), 256);
			ImGui::Text(text.c_str());
			ImGui::Text(type);
			ImGui::EndDragDropSource();
			return true;
		}
		return false;
	}
	std::string DragDropUtils::TypeFromFile(const std::string& file)
	{
		std::string ext = File::GetFileExtension(file);

		if (ext == "jpeg") return "Texture2D";
		if (ext == "jpg") return "Texture2D";
		if (ext == "png") return "Texture2D";

		if (ext == "mp3") return "AudioClip";
		if (ext == "ogg") return "AudioClip";
		if (ext == "cs") return "Script";
		if (ext == "hazard") return "World";
		if (ext == "fbx") return "Mesh";
		if (ext == "obj") return "Mesh";
		return "HAZARD_NULL";
	}
}
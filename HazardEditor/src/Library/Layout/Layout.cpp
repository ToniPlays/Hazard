#pragma once

#include "Layout.h"
#include "../Style.h"
#include <hzrpch.h>

#include <Platform/GLFW/imgui_impl_vulkan.h>
#include <Platform/GLFW/imgui_impl_glfw.h>
#include "imgui_internal.h"

#include "Platform/Rendering/OpenGL/Textures/OpenGLTexture2D.h"
#include "Platform/Rendering/OpenGL/OpenGLImage2D.h"
#include "Hazard/Rendering/RenderCommand.h"
#include "Hazard/Rendering/Texture.h"


namespace WindowLayout
{
	static std::unordered_map<const Hazard::Rendering::Image2D*, ImTextureID> m_ImageIDS;

	ImTextureID Layout::GetTextureID(const Ref<Hazard::Rendering::Image2D>& image)
	{
		if (m_ImageIDS.find(image.Raw()) != m_ImageIDS.end()) {
			return m_ImageIDS[image.Raw()];
		}

		Ref<Hazard::Rendering::OpenGL::OpenGLImage2D> glImage = image.As<Hazard::Rendering::OpenGL::OpenGLImage2D>();
		m_ImageIDS[image.Raw()] = (ImTextureID)glImage->GetID();
		return (ImTextureID)glImage->GetID();
	}
	void Layout::Texture(const Ref<Hazard::Rendering::Texture2D>& image, ImVec2 size, ImVec2 t0, ImVec2 t1)
	{
		ImGui::Image((ImTextureID)image.As<Hazard::Rendering::OpenGL::OpenGLTexture2D>()->GetID(), size, t0, t1);
	}
	void Layout::Image(const Ref<Hazard::Rendering::Image2D>& image, ImVec2 size, ImVec2 t0, ImVec2 t1)
	{
		HZR_ASSERT(image, "What image?");
		ImGui::Image(GetTextureID(image), size, t0, t1);
	}
	void Layout::Table(uint8_t columns, bool border, const char* id)
	{
		ImGui::Columns(columns, id, border);
	}
	void Layout::SameLine(float offset, float spacing)
	{
		ImGui::SameLine(offset, spacing);
	}
	void Layout::SetColumnWidth(float width)
	{
		ImGui::SetColumnWidth(ImGui::GetColumnIndex(), width);
	}
	float Layout::GetColumnWidth()
	{
		return ImGui::GetColumnWidth(ImGui::GetColumnIndex());
	}
	bool Layout::Tooltip(const char* text)
	{
		bool hovered = false;
		if (ImGui::IsItemHovered() && GImGui->HoveredIdTimer > 0.1f)
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 30.0f);
			ImGui::TextUnformatted(text);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
			hovered = true;
		}
		return hovered;
	}
	void Layout::Separator(float height)
	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetColorU32(ImGuiCol_Separator));
		ImGui::BeginChild("separ", { 0, height });
		ImGui::EndChild();
		ImGui::PopStyleColor();
	}
	void Layout::Text(const std::string& text)
	{
		ImGui::Text(text.c_str());
	}
	void Layout::TextColored(const char* text, const Color& color)
	{
		ImGui::TextColored(Style::ColorAsImVec4(color), text);
	}
	void Layout::NextLine(float height)
	{
		ImGui::Dummy({ 0, height });
	}
	void Layout::ItemWidth(float width)
	{
		ImGui::SetNextItemWidth(width);
	}
	void Layout::PushWidths(uint16_t cols)
	{
		ImGui::PushMultiItemsWidths(cols, ImGui::CalcItemWidth());
	}
	void Layout::PopWidths(uint16_t cols)
	{
		for (int i = 0; i < cols; i++) {
			PopWidth();
		}
	}
}
#pragma once

#include <hzreditor.h>
#include "Layout.h"
#include "../Style.h"

#include "imgui_internal.h"

namespace WindowLayout {

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
    void Layout::Text(const char* text)
    {
        ImGui::Text(text);
    }
    void Layout::TextColored(const char* text, const Color& color)
    {
        ImGui::TextColored(Style::ColorAsImVec4(color), text);
    }
    void Layout::NextLine(float height)
    {
        ImGui::Dummy({0, height});
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
#pragma once

#include <hzreditor.h>
#include "Layout.h"

#include "imgui_internal.h"

namespace WindowLayout {
    void Layout::Table(uint8_t columns, bool border, const char* id)
    {
        ImGui::Columns(columns, id, border);
    }
    void Layout::TableNext()
    {
        ImGui::NextColumn();
    }
    void Layout::SameLine(float offset, float spacing) 
    {
        ImGui::SameLine(offset, spacing);
    }
    void Layout::SetColumnWidth(float width)
    {
        ImGui::SetColumnWidth(ImGui::GetColumnIndex(), width);
    }
    void Layout::EndTable()
    {
        ImGui::Columns(1); 
    }
    void Layout::Text(const char* text)
    {
        ImGui::Text(text);
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
    void Layout::PopWidth()
    {
        ImGui::PopItemWidth();
    }
    void Layout::MaxWidth()
    {
        ItemWidth(ImGui::GetContentRegionAvail().x);
    }
    float Layout::GetMaxWidth()
    {
        return ImGui::GetContentRegionAvail().x;
    }
}
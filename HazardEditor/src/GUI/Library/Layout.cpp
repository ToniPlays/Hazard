#pragma once

#include <hzreditor.h>
#include "Layout.h"

namespace WindowLayout {
    void Layout::Table(uint8_t columns, bool border, const char* id)
    {
        ImGui::Columns(columns, id, border);
    }
    void Layout::TableNext()
    {
        ImGui::NextColumn();
    }
    void Layout::SameLine() 
    {
        ImGui::SameLine();
    }
    void Layout::SetColumnWidth(float width)
    {
        ImGui::SetColumnWidth(ImGui::GetColumnIndex(), width);
    }
    void Layout::EndTable()
    {
        ImGui::Columns(1);
    }
    void Layout::IDGroud(const char* id, void(*callback)())
    {
        ImGui::PushID(id);
        callback();
        ImGui::PopID();
    }
    void Layout::Treenode(const char* title, ImGuiTreeNodeFlags flags, void(*callback)())
    {

        if (ImGui::TreeNodeEx(title, flags)) {
            callback();
            ImGui::TreePop();
        }
    }
    void Layout::ContextMenu(void(*callback)())
    {
        if (ImGui::BeginPopupContextWindow(0, 1, false)) {
            callback();
            ImGui::EndPopup();
        }

    }
    bool Layout::MenuItem(const char* name)
    {
        return ImGui::MenuItem(name);
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
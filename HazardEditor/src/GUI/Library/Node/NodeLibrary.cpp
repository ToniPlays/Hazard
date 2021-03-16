#pragma once
#include <hzreditor.h>
#include "NodeLibrary.h"

#include "imgui_node_editor.h"

namespace node = ax::NodeEditor;

namespace NodeEditor {

    uint64_t NodeLibrary::id = 0;

    void NodeLibrary::DrawNode(const char* name)
    {
        node::BeginNode(id++);
        ImGui::Text(name);
        node::BeginPin(id++, node::PinKind::Input);
        ImGui::Text("Pin in");
        node::EndPin();

        ImGui::SameLine();
        node::BeginPin(id++, node::PinKind::Output);
        ImGui::Text("Pin out");
        node::EndPin();
        node::EndNode();
    }
}
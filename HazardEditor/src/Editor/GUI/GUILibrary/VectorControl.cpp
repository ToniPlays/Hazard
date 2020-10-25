#pragma once
#include <hzreditor.h>
#include "VectorControl.h"
#include "Inputs.h"

void VectorControl::DrawVec3(const char* label, Vector3<float>& vector, float resetValue, float columnWidth)
{
	ImGui::PushID(label);
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, 70);
	ImGui::Text(label);

	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2, 2));

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	Style::SetButtonColors("#DB3721", "#C3311D", "#A02818");
	Inputs::ResettableDragButton("X", vector.x, resetValue, buttonSize, 1, 0);
	ImGui::PopStyleColor(3);

	ImGui::PopItemWidth();
	ImGui::SameLine();

	Style::SetButtonColors("#5DC505", "#4A9F04", "#418B04");
	Inputs::ResettableDragButton("Y", vector.y, resetValue, buttonSize, 1, 0);
	ImGui::PopStyleColor(3);

	ImGui::PopItemWidth();
	ImGui::SameLine();

	Style::SetButtonColors("#1651F3", "#0B41D5", "#0935AE");
	Inputs::ResettableDragButton("Z", vector.z, resetValue, buttonSize, 1, 0);
	ImGui::PopStyleColor(3);

	ImGui::PopItemWidth();

	ImGui::PopStyleVar();
	ImGui::Columns(1);
	ImGui::PopID();
}

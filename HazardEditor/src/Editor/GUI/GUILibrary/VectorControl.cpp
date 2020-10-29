#pragma once
#include <hzreditor.h>
#include "VectorControl.h"
#include "Inputs.h"

void VectorControl::DravVec1(const char* label, float& value, float resetValue, float columnWidth)
{
	Inputs::BeginColumnRow(label, 1);

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	Style::SetButtonColors("#DB3721", "#C3311D", "#A02818");
	Inputs::MaxWidth();
	Inputs::ResettableDragButton("X", value, resetValue, buttonSize, 1, 0);
	ImGui::PopStyleColor(3);
	ImGui::PopItemWidth();
	Inputs::EndColumnRow();
}

void VectorControl::DrawVec2(const char* label, Vector2<float>& vector, float resetValue, float columnWidth)
{
	Inputs::BeginColumnRow(label, 2);

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
	Inputs::EndColumnRow();
}

void VectorControl::DrawVec3(const char* label, Vector3<float>& vector, float resetValue, float columnWidth)
{
	Inputs::BeginColumnRow(label, 3);

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

	Inputs::EndColumnRow();
}

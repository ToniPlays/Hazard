#pragma once
#include <hzreditor.h>
#include "Panel.h"


bool Panel::Begin(const char* title, bool& open) {
	if (!open) return false;

	ImGui::Begin(title, &open);
	return true;
}
bool Panel::Begin(std::string& title, bool& open) {
	if (!open) return false;
	ImGui::Begin(title.c_str(), &open);
	return true;
}
void Panel::End() {
	ImGui::End();
}

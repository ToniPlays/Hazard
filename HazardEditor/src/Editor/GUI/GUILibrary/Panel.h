#pragma once

#include <hzreditor.h>
#include <imgui.h>
#include <imgui_internal.h>

class Panel {
	
public:
	static bool Begin(const char* title, bool& open);
	static bool Begin(std::string& title, bool& open);
	static void End();
};
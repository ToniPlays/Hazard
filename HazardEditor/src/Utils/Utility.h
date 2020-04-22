#pragma once
#include <imgui.h>
#include "Utils/Color.h"

inline ImVec4 ColorAsImVec(Hazard::Color color) {
	return ImVec4(color.r, color.g, color.b, color.a);
}

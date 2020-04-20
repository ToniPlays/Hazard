#pragma once
#include "Core/Core.h"
#include "imgui.h"

namespace Hazard {
	class HAZARD_API Color {
	public:
		float r, g, b, a;
		Color();
		Color(int r, int g, int b, int a);
		~Color();

		static ImVec4 asImVec(Color color);
		static Color fromHEX(std::string hex);
	};
}
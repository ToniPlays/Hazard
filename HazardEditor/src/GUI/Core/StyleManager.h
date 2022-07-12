#pragma once

#include "Style.h"
#include "Platform/GLFW/FontAwesome.h"

namespace UI {
	class StyleManager {
	public:
		static void LoadStyle(const Style& style);
		static Style& GetCurrent() { return s_Current; }
	private:
		inline static Style s_Current;
	};
}
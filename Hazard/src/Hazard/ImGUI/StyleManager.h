#pragma once

#include "Style.h"
#include "../ImGui_Backend/FontAwesome.h"

namespace Hazard::ImUI 
{
	class StyleManager 
	{
	public:
		static void LoadStyle(const Style& style);
		inline static Style& GetCurrent() { return s_Current; }

	private:
		inline static Style s_Current;
	};
}

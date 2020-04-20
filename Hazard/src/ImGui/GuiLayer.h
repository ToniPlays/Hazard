#pragma once
#include "Core/Core.h"

namespace Hazard {
	class HAZARD_API GuiLayer {
	public:
		virtual void OnRender() const = 0;
		virtual void OnGetColor(Color color) {}
	protected:
		static bool show;
	};
}
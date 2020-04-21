#pragma once
#include "Core/Core.h"
#include "Utils/Color.h"

namespace Hazard {
	class HAZARD_API GuiLayer {
	public:
		virtual void OnRender() const = 0;
		virtual void OnGetColor(Color color) {}
		void OpenLayer() { isLayerOpen = true; }
		void CloseLayer() { isLayerOpen = false; }
	protected:
		bool isLayerOpen = false;
	};
}

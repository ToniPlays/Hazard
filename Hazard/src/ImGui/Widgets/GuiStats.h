#pragma once
#include "Core/Core.h"
#include "ImGui/GuiLayer.h"

namespace Hazard {
	class HAZARD_API GuiStats : public GuiLayer {
	public:
		void OnRender() const override;
		void OnGetColor(Color color) override;
	};
}
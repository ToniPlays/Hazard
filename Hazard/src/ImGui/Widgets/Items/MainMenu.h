#pragma once
#include "Core/Core.h"
#include "ImGui/GuiLayer.h"

namespace Hazard {
	class HAZARD_API MainMenu : public GuiLayer {
	public:
		void OnRender() const override;
	};
}
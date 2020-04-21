#pragma once
#include "Core/Core.h"
#include "ImGui/Editor/GuiLayer.h"

namespace Hazard {
	class HAZARD_API Performance : public GuiLayer {
	public:
		void OnRender() const override;
	};
}

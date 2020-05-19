#pragma once
#include "Core/Core.h"
#include "Modules/Module/Module.h"

namespace Hazard {

	class Input : public Module {
	public:
		Input();
		void OnEnable() override;
		bool OnEvent(Event& e) override;
	};
}
#pragma once

#include "UtilityCore.h"
#include "Event.h"

namespace Hazard::ImUI
{
	class GUIRenderable {
	public:
		virtual void Update() {};
		virtual void Render() = 0;
		virtual bool OnEvent(Event& e) = 0;

	private:

	};
}
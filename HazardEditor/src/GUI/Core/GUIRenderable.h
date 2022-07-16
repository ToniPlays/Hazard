#pragma once

#include "UtilityCore.h"
#include "Core/EditorEvent.h"

namespace UI {
	class GUIRenderable {
	public:
		virtual void Update() = 0;
		virtual void Render() = 0;
		virtual bool OnEvent(Event& e) = 0;

	private:

	};
}
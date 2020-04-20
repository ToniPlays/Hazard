#pragma once
#include "Core/Core.h"
#include "ImGui/GuiLayer.h"

namespace Hazard {
	class HAZARD_API ColorPicker {
		public:
			ColorPicker() {}
			void OnRender();
			Color color;
	private:
		friend class EditorGUI;
		GuiLayer* getterLayer;
		static bool isOpen;
	};
}
#pragma once
#include "Core/Core.h"
#include "ImGui/Editor/GuiLayer.h"

namespace Hazard {
	class HAZARD_API ColorPicker {
	public:
		ColorPicker() {}
		void OnRender();
		Hazard::Color color;
	private:
		friend class EditorGUI;
		void(*func)(Color color);
		static bool isOpen;
	};
}
#pragma once
#include "Hazard.h"

class ColorPicker {

public:
	ColorPicker() {}
	void OnRender();
	Hazard::Color color;
private:
	friend class ImGuiLayer;
	void(*func)(Hazard::Color color);
	static bool isOpen;
};

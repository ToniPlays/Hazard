#pragma once
#include "Hazard.h"

class ColorPicker {

public:
	ColorPicker() {}
	void OnRender();
	
	Hazard::Color& color = Hazard::Color();
private:
	friend class Editor;
	void(*func)(Hazard::Color color);
	static bool isOpen;
};

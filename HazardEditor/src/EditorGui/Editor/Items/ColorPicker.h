#pragma once
#include <HzrEditor.h>

class ColorPicker {

public:
	ColorPicker() {}
	void OnRender();
	Hazard::Color color;
private:
	friend class EditorGUI;
	void(*func)(Hazard::Color color);
	static bool isOpen;
};

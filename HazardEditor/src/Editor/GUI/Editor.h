#pragma once
#include "Hazard.h"
#include "Layers/Layer.h"
#include "Editor/GUI/Items/ColorPicker.h"

class Editor : public Hazard::Module {
public:
	Editor();
	~Editor();

	bool OnEnabled();
	void Render();
	bool OnDisabled();
	void PushLayer(Layer* layer);
	static void OpenColorPicker(Hazard::Color color, void(*func)(Hazard::Color color));
private:
	static ColorPicker* colorPicker;
	void Begin();
	void End();
	std::vector<Layer*> layers;
};
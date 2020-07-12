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

	template<typename T>
	static bool SetLayerOpen(bool open) {
		for (Layer* layer : instance->layers) {
			if (dynamic_cast<T*>(layer)) {
				layer->SetLayerOpen(open);
				return true;
			}
		}
		return false;
	}
	static void OpenColorPicker(Hazard::Color color, void(*func)(Hazard::Color color));
private:
	static ColorPicker* colorPicker;
	static Editor* instance;
	void Begin();
	void End();
	std::vector<Layer*> layers;
};
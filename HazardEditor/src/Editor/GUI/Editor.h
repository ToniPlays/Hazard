#pragma once
#include "Hazard.h"
#include "Layers/Layer.h"

class Editor : public Hazard::Module {
public:
	Editor();
	~Editor();

	void OnEnabled();
	void Render();
	void OnDisabled();
	void PushLayer(Layer* layer);

private:
	void Begin();
	void End();
	std::vector<Layer*> layers;
};
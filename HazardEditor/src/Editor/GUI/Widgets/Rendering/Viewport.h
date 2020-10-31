#pragma once

#include "Editor/GUI/Layers/Layer.h"

class Viewport : public Layer {
public:
	Viewport();
	void Render() override;
	bool OnEnabled() override;
	std::string MenuPlacement() { return "View/General"; }

private:
	Hazard::RenderEngine* renderer;
	float width = -1.0f, height = -1.0f;
	static bool ShowLog;
	static bool ShowWarn;
	static bool ShowError;
};


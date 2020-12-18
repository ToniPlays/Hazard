#pragma once

#include "Editor/GUI/Layer.h"
#include "Hazard.h"

class Viewport : public Layer {
public:
	Viewport();
	~Viewport();

	void Render() override;
	bool OnEnabled() override;
	std::string MenuPlacement() { return "View/General"; }

private:

	RenderEngine* renderer;

	float width = -1.0f, height = -1.0f;
	static bool ShowLog;
	static bool ShowWarn;
	static bool ShowError;
};


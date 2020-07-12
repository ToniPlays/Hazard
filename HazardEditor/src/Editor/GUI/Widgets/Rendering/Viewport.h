#pragma once

#include "Editor/GUI/Layers/Layer.h"

class Viewport : public Layer {
public:
	Viewport();
	void Render() override;
	bool OnEnabled() override;

private:
	float width = 0.0f, height = 0.0f;
	static bool ShowLog;
	static bool ShowWarn;
	static bool ShowError;
};


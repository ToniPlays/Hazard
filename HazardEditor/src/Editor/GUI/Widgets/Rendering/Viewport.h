#pragma once

#include "Editor/GUI/Layers/Layer.h"

class Viewport : public Layer {
public:
	Viewport();
	void Render() override;
	bool OnEnabled() override;

private:
	static bool layerOpen;
	static bool ShowLog;
	static bool ShowWarn;
	static bool ShowError;
};


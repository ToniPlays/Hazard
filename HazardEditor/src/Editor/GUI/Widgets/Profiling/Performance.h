#pragma once
#include "Editor/GUI/Layers/Layer.h"

class Performance : public Layer {
public:
	Performance();
	void Render() override;
	bool OnEnabled() override;
	std::string MenuPlacement() { return "View/Analytics"; }
};


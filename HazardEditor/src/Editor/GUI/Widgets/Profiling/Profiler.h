#pragma once

#include "Editor/GUI/Layers/Layer.h"

class Profiler : public Layer {
public:
	Profiler();
	bool OnEnabled();
	void Render() override;
	std::string MenuPlacement() { return "View/Analytics"; }
};


#pragma once
#include "Editor/GUI/Layer.h"

class Performance : public Layer {
public:
	Performance();
	~Performance() override;

	void Render() override;
	bool OnEnabled() override;
	std::string MenuPlacement() { return "View/Analytics"; }
};


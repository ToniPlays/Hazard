#pragma once
#include "Editor/GUI/Layers/Layer.h"

class Performance : public Layer {
public:
	Performance();
	void Render() override;
private:
	static bool layerOpen;
};


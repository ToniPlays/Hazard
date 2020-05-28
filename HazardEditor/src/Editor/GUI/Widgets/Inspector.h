#pragma once

#include "Editor/GUI/Layers/Layer.h"

class Inspector : public Layer {
public:
	Inspector();
	void Render() override;
private:
	static bool layerOpen;
};


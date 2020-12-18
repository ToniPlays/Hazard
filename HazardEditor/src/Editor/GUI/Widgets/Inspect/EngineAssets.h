#pragma once
#include "Editor/GUI/Layer.h"

class EngineAssets : public Layer {
public:
	EngineAssets();
	~EngineAssets() override;

	void Render() override;
	std::string MenuPlacement() { return "View/Assets";  }

};


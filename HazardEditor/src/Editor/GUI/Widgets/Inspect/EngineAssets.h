#pragma once
#include "Editor/GUI/Layers/Layer.h"

class EngineAssets : public Layer {
public:
	EngineAssets();
	void Render() override;
	void DrawTextures(TextureData* data);
	std::string MenuPlacement() { return "View/Assets";  }
};


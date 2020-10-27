#pragma once

#include "Editor/GUI/Layers/Layer.h"
#include "Editor/Core/HazardProject.h"

class NewProject : public Layer {
public:
	NewProject();
	bool OnEnabled() override;
	void Render() override;
	std::string MenuPlacement() { return "File"; }
};
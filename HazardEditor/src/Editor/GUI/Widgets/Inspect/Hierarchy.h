#pragma once

#include "Editor/GUI/Layers/Layer.h"

using namespace Hazard;

class Hierarchy : public Layer {

public:
	Hierarchy();
	~Hierarchy() override;
	bool OnEnabled() override;
	void Render() override;

	std::string MenuPlacement() override { return "View/General"; }
private:
	void DrawEntities(std::vector<Entity*> entities);

	Entity* entityContext = nullptr;
};
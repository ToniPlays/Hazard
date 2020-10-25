#pragma once
#include "Editor/GUI/Layers/Layer.h"


using namespace Hazard;

class Inspector : public Layer {

public:
	Inspector();
	void Render() override;
	void OpenContext(Entity* entity);
	std::string MenuPlacement() { return "View/General"; }

private:
	Entity* context = nullptr;
	bool locked = false;
};


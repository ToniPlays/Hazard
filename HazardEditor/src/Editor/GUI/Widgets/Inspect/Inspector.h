#pragma once
#include "Editor/GUI/Layer.h"


using namespace Hazard;

class Inspector : public Layer {

public:
	Inspector();
	~Inspector() override;

	void Render() override;
	void SetContext(ECS::Entity entity) { context = entity; };

	std::string MenuPlacement() { return "View/General"; }

private:

	ECS::Entity context;
	bool locked = false;
};


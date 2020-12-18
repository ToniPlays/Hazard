#pragma once

#include "Editor/GUI/Layer.h"

using namespace Hazard;

class Hierarchy : public Layer {

public:
	Hierarchy();
	~Hierarchy() override;
	bool OnEnabled() override;
	void Render() override;
	void DrawEntity(Hazard::ECS::Entity entity);

	std::string MenuPlacement() override { return "View/General"; }
private:
	ECS::Scene* context = nullptr;
	ECS::SceneManager* manager = nullptr;
	ECS::Entity selectionContext = {};
};
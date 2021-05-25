#pragma once

#include "Hazard.h"
#include "Hazard/Core/EntryPoint.h"

using namespace Hazard;
class HazardPlayer : public Application {

public:

	HazardPlayer();
	~HazardPlayer();

	void PreInit() override;
	void Init() override;
	void Update() override;
	bool OnEvent(Event& e) override;
};

Hazard::Application* Hazard::CreateApplication() {
	return new HazardPlayer();
}
#pragma once
#if 1
#include "Hazard.h"
#include "Hazard/Core/EntryPoint.h"

class HazardLauncher : public Hazard::Application {
public:
	HazardLauncher() = default;
	~HazardLauncher() = default;

	void PreInit() override;
	void Init() override;
};

Hazard::Application* Hazard::CreateApplication()
{
	return hnew HazardLauncher();
}
#endif

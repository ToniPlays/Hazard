#pragma once
#include <Hazard.h>

class HazardEditor : public Hazard::Application {
public:
	HazardEditor();
	~HazardEditor();
};

Hazard::Application* Hazard::CreateApplication() {
	return new HazardEditor();
}
#pragma once
#include "HzrEditor.h"

class HazardEditor : public Hazard::Application {
public:
	HazardEditor();
	~HazardEditor();

	void Start() override;
};

Hazard::Application* Hazard::CreateApplication() {
	return new HazardEditor();
}
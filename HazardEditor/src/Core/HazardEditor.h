#pragma once
#include <HzrEditor.h>
#include "Core/EntryPoint.h"

class HazardEditor : public Hazard::Application {

public:
	HazardEditor();
	~HazardEditor();

	void Start();
};

Hazard::Application* Hazard::CreateApplication() {
	return new HazardEditor();
}

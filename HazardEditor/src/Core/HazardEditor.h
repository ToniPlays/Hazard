#pragma once

#include "Hazard.h"
#include "Hazard/Core/EntryPoint.h"

using namespace Hazard;
class EditorApplication : public Application {

public:

	EditorApplication();
	~EditorApplication();

	void Init() override;
};

Hazard::Application* Hazard::CreateApplication() {
	return new EditorApplication();
}
#pragma once

#include "Hazard.h"
#include "Hazard/Core/EntryPoint.h"

class EditorApplication : public Hazard::Application {

public:
	EditorApplication();
	~EditorApplication();

	void Awake();
};

Hazard::Application* Hazard::CreateApplication() {
	return new EditorApplication();
}
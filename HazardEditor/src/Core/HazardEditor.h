#pragma once

#include "Hazard.h"
#include "Hazard/Core/EntryPoint.h"

class EditorApplication : public Application {

public:
	EditorApplication();
	~EditorApplication();

	void PreInit() override;
	void Init() override;
	bool OnEvent(Event& e) override;
};

Hazard::Application* Hazard::CreateApplication() {
	return new EditorApplication();
}
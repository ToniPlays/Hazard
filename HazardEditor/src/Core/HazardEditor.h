#pragma once

#include "Hazard.h"
#include "Hazard/Core/EntryPoint.h"

class EditorApplication : public Hazard::Application {

public:
	EditorApplication() = default;
	~EditorApplication() = default;

	void PreInit() override;
	void Init() override;
	bool OnEvent(Hazard::Event& e) override;
};

Hazard::Application* Hazard::CreateApplication() 
{
	return new EditorApplication();
}
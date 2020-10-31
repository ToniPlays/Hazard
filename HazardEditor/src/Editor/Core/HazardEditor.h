#pragma once

#include "Hazard.h"
#include "Hazard/Core/EntryPoint.h"


class EditorApplication : public Hazard::Application {

public:
	
	EditorApplication();
	~EditorApplication();

	void Awake() override;
	void Update() override;
	void OnEvent(Hazard::Event& e) override;
};

Hazard::Application* Hazard::CreateApplication() {
	return new EditorApplication();
}
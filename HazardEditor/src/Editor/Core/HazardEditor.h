#pragma once

#include "Hazard.h"
#include "Hazard/Core/EntryPoint.h"


class EditorApplication : public Hazard::Application {

public:
	
	EditorApplication();
	~EditorApplication();

	void Awake() override;
	void Update() override;
	bool OnEvent(Hazard::Event& e) override;
	void OnClose() override;
};

Hazard::Application* Hazard::CreateApplication() {
	return new EditorApplication();
}
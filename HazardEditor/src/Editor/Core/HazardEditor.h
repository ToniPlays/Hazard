#pragma once

#include "Hazard.h"
#include "Hazard/Core/EntryPoint.h"

class EditorApplication : public Hazard::Application {

public:
	EditorApplication();
	~EditorApplication();

	void Awake();
	void OnEvent(Hazard::Event& e);
	bool CameraZoom(Hazard::Event& e);
};

Hazard::Application* Hazard::CreateApplication() {
	return new EditorApplication();
}
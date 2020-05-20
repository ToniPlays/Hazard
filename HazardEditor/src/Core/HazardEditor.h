#pragma once
#include <HzrEditor.h>
#include "Core/EntryPoint.h"

class HazardEditor : public Hazard::Application {
	#define BIND(x) std::bind(&HazardEditor::x, this, std::placeholders::_1)
public:
	HazardEditor();
	~HazardEditor();

	void Start();
	void OnEvent(Hazard::Event& e);
	bool TestEvent(Hazard::KeyPressedEvent& e);
private:
	std::unordered_map<std::string, Hazard::RenderTexture*> renderTextures;
};

Hazard::Application* Hazard::CreateApplication() {
	return new HazardEditor();
}

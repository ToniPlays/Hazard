#pragma once
#include "Hazard.h"

class EditorGUI : public Hazard::Layer {
public:
	EditorGUI();
	~EditorGUI();
	void OnAttach();
	void OnImGuiRender();
};
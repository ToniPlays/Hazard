#pragma once

#include "HzrEditor.h"
#include "core/Layers/Layer.h"
#include "GuiLayer.h"
#include "Items/ColorPicker.h"
#include "EditorGui/Editor/Items/EditorStyle.h"

class EditorGUI : public Hazard::Layer {
public:
	EditorGUI();
	~EditorGUI();
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void PushLayer(std::string name, GuiLayer* layer) { layers[name] = layer; }
	static void OpenPicker(void(*func)(Hazard::Color color), Hazard::Color color);
	static void OpenLayer(std::string name);

private:
	void Begin();
	void End();

private:
	static ColorPicker* colorPicker;
	static std::unordered_map<std::string, GuiLayer*> layers;
};

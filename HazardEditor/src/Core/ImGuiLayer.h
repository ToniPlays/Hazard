#pragma once

#include "Hazard.h"
#include "imgui.h"
#include "GuiLayer.h"
#include "Gui/Component/ColorPicker.h"

class ImGuiLayer : public Hazard::Module {
public:

	ImGuiLayer();
	~ImGuiLayer();
	void OnEnable() override;
	void OnDisable() override;
	void OnRender() override;
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
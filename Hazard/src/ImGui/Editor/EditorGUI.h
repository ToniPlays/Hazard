#pragma once
#include "Core/Core.h"
#include "core/Layers/Layer.h"
#include "GuiLayer.h"
#include "Items/ColorPicker.h"


namespace Hazard {
	class HAZARD_API EditorGUI : public Hazard::Layer {
	public:
		EditorGUI();
		~EditorGUI();
		void OnAttach();
		void OnDetach();
		void OnImGuiRender();
		void PushLayer(std::string name, GuiLayer* layer) { layers[name] = layer; }
		static void OpenPicker(GuiLayer* layer, Hazard::Color color);
		static void OpenLayer(std::string name);

	private:
		static ColorPicker* colorPicker;
		static std::unordered_map<std::string, GuiLayer*> layers;
	};
}
#pragma once
#include "Core/Core.h"
#include "core/Layers/Layer.h"
#include "GuiLayer.h"
#include "ImGui/Widgets/Items/ColorPicker.h"

namespace Hazard {
	class HAZARD_API EditorGUI : public Layer {
	public:
		EditorGUI();
		~EditorGUI();
		void OnAttach();
		void OnDetach();
		void OnImGuiRender();

		void PushLayer(GuiLayer* layer) { layers.emplace_back(layer); }
		static void OpenPicker(GuiLayer* layer);
	private:
		static ColorPicker* colorPicker;
		std::vector<GuiLayer*> layers;
	};
}
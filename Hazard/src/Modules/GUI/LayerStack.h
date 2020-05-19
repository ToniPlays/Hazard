#pragma once
#include "Core/Core.h"
#include "Layer.h"
#include <vector>
#include "Modules/Module/Module.h"

namespace Hazard {
	class HAZARD_API LayerStack : public Module {
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* layer);
		std::vector<Layer*>::iterator begin() { return layers.begin(); }
		std::vector<Layer*>::iterator end() { return layers.end(); }

		void OnRender();
	private:
		std::vector<Layer*> layers;
		unsigned int LayerInsert = 0;
	};
}

#include <hzrpch.h>
#include "LayerStack.h"

namespace Hazard {
	LayerStack::LayerStack()
	{

	}
	LayerStack::~LayerStack()
	{
		for (Layer* layer : layers) {
			delete layer;
		}
	}
	void LayerStack::PushLayer(Layer* layer)
	{
		layers.emplace(layers.begin() + LayerInsert, layer);
		LayerInsert++;
		layer->OnAttach();
	}
	void LayerStack::PushOverlay(Layer* layer)
	{
		layers.emplace_back(layer);
		layer->OnAttach();
	}
	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(layers.begin(), layers.end(), layer);
		if (it != layers.end()) {
			layers.erase(it);
			layer->OnDetach();
			LayerInsert--;
		}
	}
	void LayerStack::PopOverlay(Layer* layer)
	{
		auto it = std::find(layers.begin(), layers.end(), layer);
		if (it != layers.end()) {
			layer->OnDetach();
			layers.erase(it);
		}
	}
}
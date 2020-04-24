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
	//Add layer to LayerStack

	void LayerStack::PushLayer(Layer* layer)
	{
		layers.emplace(layers.begin() + LayerInsert, layer);
		LayerInsert++;
		layer->OnAttach();
	}
	//Add overlay to LayerStack
	void LayerStack::PushOverlay(Layer* layer)
	{
		layers.emplace_back(layer);
		layer->OnAttach();
	}
	//Remove layer in LayerStack
	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(layers.begin(), layers.end(), layer);
		if (it != layers.end()) {
			layers.erase(it);
			layer->OnDetach();
			LayerInsert--;
		}
	}
	//Remove overlay in LayerStack
	void LayerStack::PopOverlay(Layer* layer)
	{
		auto it = std::find(layers.begin(), layers.end(), layer);
		if (it != layers.end()) {
			layer->OnDetach();
			layers.erase(it);
		}
	}
}
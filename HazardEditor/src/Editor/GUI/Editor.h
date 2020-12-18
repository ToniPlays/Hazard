#pragma once
#include "Hazard.h"
#include "Layer.h"
#include "Editor/GUI/Widgets/MainMenu.h"

using namespace Hazard;

class Editor : public Hazard::Module {
public:
	Editor();
	~Editor();

	bool OnEnabled();
	void Render();
	bool OnDisabled();
	void PushLayer(Layer* layer);

	void Register(Layer* layer);

	template<typename T>
	static bool SetLayerOpen(bool open) {
		for (Layer* layer : instance->layers) {
			if (dynamic_cast<T*>(layer)) {
				layer->SetLayerOpen(open);
				return true;
			}
		}
		return false;
	}
	template <typename T>
		static T* GetLayer() {
		for (Layer* layer : instance->layers) {
			if (dynamic_cast<T*>(layer)) {
				return (T*)layer;
			}
		}
		return nullptr;
	}
private:
	static Editor* instance;

	MainMenu mainMenu;
	std::vector<Layer*> layers;
	void Begin();
	void End();
};
#pragma once

#include "Hazard.h"

class Layer {
public:
	Layer(std::string n) : name(n) {}
	virtual ~Layer() {};

	virtual void Update() {};
	virtual void Render() {};

	virtual bool OnEnabled() { return true; }
	virtual bool OnDisabled() { return true; }

	void SetLayerOpen(bool open) { isLayerOpen = open; }
	virtual std::string MenuPlacement() { return ""; };
	std::string GetName() { return name; }
protected:
	bool isLayerOpen = true;
	std::string name;
};
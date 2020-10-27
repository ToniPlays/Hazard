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
	virtual std::string MenuPlacement() { return ""; };

	void SetLayerOpen(bool open) { isLayerOpen = open; };

	std::string GetName() { return name; }
protected:
	bool isLayerOpen = true;
	std::string name;
};
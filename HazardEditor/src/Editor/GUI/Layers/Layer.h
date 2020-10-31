#pragma once

#include "Hazard.h"

class Layer {
public:
	Layer(const char* n) : name(n) {}
	virtual ~Layer() {};

	virtual void Update() {};
	virtual void Render() {};

	virtual bool OnEnabled() { return true; }
	virtual bool OnDisabled() { return true; }
	virtual std::string MenuPlacement() { return ""; };

	void SetLayerOpen(bool open) { isLayerOpen = open; };

	const char* GetName() { return name; }
protected:
	bool isLayerOpen = true;
	const char* name;
};
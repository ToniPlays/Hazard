#pragma once

#include "Hazard.h"

class Layer {
public:
	virtual ~Layer() {};

	virtual void Awake() {};
	virtual void Start() {};

	virtual void Update() {};
	virtual void Render() {};

	virtual bool OnEnabled() { return true; };
	virtual bool OnDisabled() { return true; };
};
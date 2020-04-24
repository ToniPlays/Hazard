#pragma once
#include "Hazard.h"

class GuiLayer {
public:
	virtual void OnRender() const = 0;
	virtual void OpenLayer() const = 0;
	virtual void CloseLayer() const = 0;
};


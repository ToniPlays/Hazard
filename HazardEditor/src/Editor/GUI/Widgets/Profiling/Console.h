#pragma once

#include "Editor/GUI/Layers/Layer.h"
#include "Editor/Core/Analytics/Debug.h"

class Console : public Layer {
public:
	Console();
	void Render() override;
	bool OnEnabled() override;
	
private:
	static bool ShowLog;
	static bool ShowWarn;
	static bool ShowError;
};


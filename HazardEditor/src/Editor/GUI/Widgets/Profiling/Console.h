#pragma once

#include "Editor/GUI/Layers/Layer.h"
#include "Editor/Core/Analytics/Debug.h"

class Console : public Layer {
public:
	Console();
	~Console() override;

	void Render() override;
	bool OnEnabled() override;
	std::string MenuPlacement() { return "View/Analytics"; }
	
private:
	static bool ShowLog;
	static bool ShowWarn;
	static bool ShowError;
};


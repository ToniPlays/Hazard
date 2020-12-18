#pragma once

#include "Editor/GUI/Layer.h"

class Profiler : public Layer {
public:
	Profiler();
	~Profiler() override;

	bool OnEnabled();
	void Render() override;
	void DisplayLog(Hazard::ProfiledFn* parent);
	std::string MenuPlacement() { return "View/Analytics"; }
private:
	Logger* logger = nullptr;
};


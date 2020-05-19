#pragma once
#include "HzrEditor.h"
#include "Core/GuiLayer.h"

class Hierarchy : public GuiLayer {
public:
	void OnRender() const override;
	void OpenLayer() const override;
	void CloseLayer() const override;
private:
	static bool layerOpen;
};
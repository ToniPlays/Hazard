#pragma once
#include "HzrEditor.h"
#include "EditorGui/Editor/GuiLayer.h"

class Performance : public GuiLayer {
public:
	void OnRender() const override;
	void OpenLayer() const override { layerOpen = true; }
	void CloseLayer() const override { layerOpen = false; }
private:
	static bool layerOpen;
};


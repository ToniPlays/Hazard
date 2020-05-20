#pragma once
#include "HzrEditor.h"
#include "Core/GuiLayer.h"


class Viewport : public GuiLayer {
public:
	Viewport();
	void OnRender() const override;
	void OpenLayer() const override;
	void CloseLayer() const override;
private:
	static bool isOpen;
	static bool useRender;
	Hazard::RenderTexture* texture;
};

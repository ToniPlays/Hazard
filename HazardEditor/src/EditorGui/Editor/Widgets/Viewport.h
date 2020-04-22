#pragma once
#include "HzrEditor.h"
#include "EditorGui/Editor/GuiLayer.h"


class Viewport : public GuiLayer {
public:
	void OnRender() const override;
	void OpenLayer() const override { isOpen = true; }
	void CloseLayer() const override { isOpen = false; }
private:
	static bool isOpen;
};

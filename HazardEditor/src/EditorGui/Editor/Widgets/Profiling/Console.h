#pragma once

#include "HzrEditor.h"
#include "EditorGui/Editor/GuiLayer.h"

class Console : public GuiLayer {
public:
	Console(Debug* d) : debug(d) {}
	void OnRender() const override;
	void OpenLayer() const override;
	void CloseLayer() const override;
private:
	Debug* debug;
	static bool isOpen;
	static bool showErrors;
	static bool showWarnings;
	static bool showInfo;
};
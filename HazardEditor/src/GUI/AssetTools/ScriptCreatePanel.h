#pragma once

#include "Hazard.h"
#include "Hazard/ImGUI/Panel.h"




class ScriptCreatePanel : public Hazard::ImUI::Panel 
{
public:
	ScriptCreatePanel() = default;
	~ScriptCreatePanel() = default;

	void OnPanelRender() override;
private:
	
};
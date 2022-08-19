#pragma once

#include "Hazard.h"

#include "Event.h"
#include "Editor/EditorScriptManager.h"


class HazardEditorApplication : public Hazard::Application {

public:
	HazardEditorApplication() = default;
	~HazardEditorApplication() = default;

	void PreInit() override;
	void Init() override;
	bool OnEvent(Event& e) override;

	Editor::EditorScriptManager& GetScriptManager() { return m_ScriptManager; };

private:

	Editor::EditorScriptManager m_ScriptManager;
};

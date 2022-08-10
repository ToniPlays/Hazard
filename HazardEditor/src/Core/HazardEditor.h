#pragma once

#include "Hazard.h"

#include "Event.h"
#include "Editor/EditorScriptManager.h"


class EditorApplication : public Hazard::Application {

public:
	EditorApplication() = default;
	~EditorApplication() = default;

	void PreInit() override;
	void Init() override;
	bool OnEvent(Event& e) override;

	Editor::EditorScriptManager& GetScriptManager() { return m_ScriptManager; };

private:

	Editor::EditorScriptManager m_ScriptManager;
};

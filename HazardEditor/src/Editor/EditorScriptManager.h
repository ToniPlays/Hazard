#pragma once

#include "HazardRendererCore.h"
#include "GUI/Debug/Console.h"

namespace Editor {

	class EditorScriptManager {
	public:
		EditorScriptManager() = default;

		void GenerateProjectFiles();
		void CompileSource();
		void ReloadAssebmly();

		void RecompileAndLoad();

		bool ReloadOnFocus() { return m_ReloadOnFocus; }
		void SetReloadOnFocus(bool enabled) { m_ReloadOnFocus = enabled; }

		bool OnEvent(Event& e);
		bool OnWindowFocusEvent(WindowFocusEvent& e);

	private:
		std::vector<UI::ConsoleMessage> ParseBuildResult(const std::string& source);
	private:
		bool m_ReloadOnFocus = false;
	};
}
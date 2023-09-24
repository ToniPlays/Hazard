#pragma once

#include "GUI/Debug/Console.h"

namespace Editor 
{
	struct ScriptPreprocessor
	{
		std::string Name;
		std::string Availability;
	};

	class EditorScriptManager 
	{
	public:
		EditorScriptManager() = default;

		void Init();

		void GenerateProjectFiles();
		void CompileSource();
		void ReloadAssembly();

		void RecompileAndLoad();

		bool ReloadOnFocus() { return m_ReloadOnFocus; }
		void SetReloadOnFocus(bool enabled) { m_ReloadOnFocus = enabled; }

		bool OnEvent(Event& e);
		bool OnWindowFocusEvent(WindowFocusEvent& e);

		std::vector<ScriptPreprocessor>& GetPreprocessorDefines() { return m_Preprocessors; }

	private:
		std::vector<UI::ConsoleMessage> ParseBuildResult(const std::string& source);

	private:
		bool m_ReloadOnFocus = false;
		std::vector<ScriptPreprocessor> m_Preprocessors;
	};
}
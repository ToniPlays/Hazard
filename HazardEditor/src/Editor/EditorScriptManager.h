#pragma once

#include "HazardRendererCore.h"

namespace Editor {

	class EditorScriptManager {
	public:
		EditorScriptManager() = default;

		void GenerateProjectFiles();
		void CompileSource();
		void ReloadAssebmly();

		void RecompileAndLoad() {
			GenerateProjectFiles();
			CompileSource();
			ReloadAssebmly();
		}

		bool ReloadOnFocus() { return m_ReloadOnFocus; }
		void SetReloadOnFocus(bool enabled) { m_ReloadOnFocus = enabled; }

		bool OnEvent(Event& e);
		bool OnWindowFocusEvent(WindowFocusEvent& e);

	private:
		bool m_ReloadOnFocus = true;
	};
}
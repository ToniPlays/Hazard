#pragma once

namespace Editor {

	struct InterfaceSettings {
		bool showStats = true;
	};

	class EditorSettings 
	{
	public:
		static InterfaceSettings GetInterfaceSettings() { return s_InterfaceSettings; }
	private:
		static InterfaceSettings s_InterfaceSettings;
	};
}
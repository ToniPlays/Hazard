#pragma once


namespace Hazard::Scripting {

	class ScriptRegistry {
	public:
		static void RegisterAll();
	private:
		static void Register(std::vector<std::pair<const char*, void*>> methods);
	};
}
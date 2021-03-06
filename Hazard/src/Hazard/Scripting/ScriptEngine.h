#pragma once

#include "Hazard/Module/Module.h"

namespace Hazard::Scripting {

	class ScriptEngine : public Module::Module {
	public:
		ScriptEngine();

		~ScriptEngine();

		void Init() override;
		void Close() override;
		void InitAssembly(const char* assemblyPath);
		void LoadRuntimeAssembly(const char* path);

		void ReloadAssembly() { ReloadAssembly(assemblyPath.c_str()); };
		void ReloadAssembly(const char* assemblyPath);

	private:
		void MonoInit();
		void MonoShutdown();

	private:
		std::string assemblyPath;
	};
}
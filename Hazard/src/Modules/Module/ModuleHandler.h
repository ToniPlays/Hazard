#pragma once
#include "Core/Core.h"
#include "Module.h"

namespace Hazard {
	class HAZARD_API ModuleHandler {
		
	public:

		ModuleHandler();
		~ModuleHandler();

		void PushModule(Module* module);
		void PopModule(Module* module);
		void Update();
		void LateUpdate();
		void OnRender();
		void OnEvent(Event& e);
		std::vector<Module*> GetAll() {	return modules; }

		template <typename T>
		T* GetModule() {
			for (Module* module : modules) {
				if (dynamic_cast<T*>(module)) {
					return (T*)module;
				}
			}
			return nullptr;
		}

	private:
		std::vector<Module*> modules;
	};
}
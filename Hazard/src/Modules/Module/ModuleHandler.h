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

		template <typename T>
		T* GetModule() {
			T type;
			for (Module* module : modules) {
				if (typeid(*module) == typeid(type)) {
					return static_cast<T*>(module);
				}
			}
			std::stringstream ss;
			ss << "Module of type " << typeid(type).name();

			HZR_CORE_WARN(ss.str());
			return nullptr;
		}
		std::vector<Module*> GetAll() {	return modules; }

	private:
		std::vector<Module*> modules;
	};
}
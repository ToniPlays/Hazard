#pragma once
#include "Hazard/Core/Core.h"
#include "Module.h"

namespace Hazard::Module {

	class ModuleHandler {
	public:
		ModuleHandler();
		~ModuleHandler();

		void PreInit();
		void Init();
		void Update();
		void Render();
		void Close();

		template<typename T>
		T& AddModule() {
			T* m = new T();
			modules.push_back(m);
			HZR_CORE_INFO("Added module " + std::string(typeid(T).name()));
			return *m;
		};

		template<typename T>
		T& GetModule()
		{
			for (Module* m : modules) {
				if (dynamic_cast<T*>(m))
					return (T&)*m;
			}
		}
		template<typename T>
		T& GetModule(bool& found)
		{
			for (Module* m : modules) {
				if (dynamic_cast<T*>(m)) {
					found = true;
					return (T&)*m;
				}
			}
			found = false;
		}

	private:
		std::vector<Module*> modules;
	};
}
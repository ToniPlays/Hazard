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
		void AddModule() {
			T* m = new T();
			modules.push_back(m);
		};

		template<typename T>
		T& GetModule()
		{
			for (Module* m : modules) {
				if (dynamic_cast<T*>(m))
					return (T&)*m;
			}
		}

	private:
		std::vector<Module*> modules;
	};
}
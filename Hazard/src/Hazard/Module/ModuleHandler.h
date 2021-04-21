#pragma once
#include "Hazard/Core/Core.h"
#include "Module.h"

namespace Hazard::Module {

	class ModuleHandler {
	public:
		ModuleHandler();
		~ModuleHandler();

		void Update();
		void Render();
		void Close();

		template<typename T>
		T& AddModule() {
			T* m = new T();
			m_Modules.push_back(m);
			m->PreInit();
			m->Init();
			return *m;
		};

		template<typename T>
		T& GetModule()
		{
			for (Module* m : m_Modules) {
				if (dynamic_cast<T*>(m))
					return (T&)*m;
			}
		}
		template<typename T>
		T& GetModule(bool& found)
		{
			for (Module* m : m_Modules) {
				if (dynamic_cast<T*>(m)) {
					found = true;
					return (T&)*m;
				}
			}
			found = false;
		}
	private:
		std::vector<Module*> m_Modules;
	};
}
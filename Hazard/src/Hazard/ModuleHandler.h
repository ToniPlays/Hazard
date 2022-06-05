#pragma once
#include "Hazard/Core/Core.h"
#include "Module.h"

namespace Hazard {

	class ModuleHandler {
	public:
		ModuleHandler();
		~ModuleHandler();

		void Update();
		void Render();
		void PostRender();
		void Close();

		template<typename T, typename... Args>
		T* AddModule(Args... args)
		{
			T* m = new T(std::forward<Args>(args)...);
			m_Modules.push_back(m);
			m->PreInit();
			m->Init();
			return m;
		};

		template<typename T>
		T* GetModule()
		{
			for (Module* m : m_Modules) {
				if (dynamic_cast<T*>(m))
					return (T*)m;
			}
			return nullptr;
		}
		template<typename T>
		bool HasModule()
		{
			for (Module* m : m_Modules) {
				if (dynamic_cast<T*>(m)) {
					return true;
				}
			}
			return false;
		}
		bool OnEvent(Event& e)
		{
			for (Module* m : m_Modules) 
			{
				if(m->OnEvent(e))
					return true;
			}
			return false;
		}
	private:
		std::vector<Module*> m_Modules;
	};
}
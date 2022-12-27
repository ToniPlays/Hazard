#pragma once
#include "Hazard/Core/Core.h"
#include "Module.h"

namespace Hazard {

	class ModuleHandler
    {
	public:
		ModuleHandler();
		~ModuleHandler();

		void InitializeAll();

		void PreUpdate();
		void Update();
		void PostUpdate();
		void PreRender();
		void Render();
		void PostRender();
		void Close();

		template<typename T, typename... Args>
		T* AddModule(Args... args)
		{
			T* m = hnew T(std::forward<Args>(args)...);
			m_Modules.push_back(m);
			m->PreInit();
			if (m_HasInitialized) m->Init();
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
				if (m->OnEvent(e))
					return true;
			}
			return false;
		}
	private:
		std::vector<Module*> m_Modules;
		bool m_HasInitialized = false;
	};
}

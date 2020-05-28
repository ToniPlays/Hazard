#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Core/Events/Event.h"
#include "Module.h"
#include <iostream>
#include <vector>

namespace Hazard {
	class HAZARD_API ModuleHandler {

		friend class Application;
		friend class HazardLoop;

	public:
		ModuleHandler();
		~ModuleHandler();

		template <typename T>
		static T* GetModule() {
			for (Module* module : modules) {
				if (dynamic_cast<T*>(module)) {
					return (T*)module;
				}
			}
			return nullptr;
		}

		static void PushModule(Module* module);
		static bool PopModule(Module* module);
		static std::vector<Module*> GetAll() { return modules; }

	private:

		void Awake();
		void Start();
		void Update();
		void LateUpdate();
		void OnRender();
		void OnEvent(Event& e);
		void OnClose();

	private:
		static std::vector<Module*> modules;
	};

}
#pragma once

#include "UtilityCore.h"
#include "Event.h"

#include "GUIRenderable.h"

namespace Hazard::ImUI
{
	class PanelManager {
	public:

		PanelManager();

		void Update();
		void Render();

		bool OnEvent(Event& e) {
			for (auto& renderable : m_Renderables) {
				if (renderable->OnEvent(e)) return true;
			}
			return false;
		}

		template<typename T>
		static T& AddRenderable() 
		{
			T* renderable = hnew T();
			s_Instance->m_Renderables.push_back(renderable);
			return *renderable;
		}
		template<typename T>
		static T* GetRenderable() {
			for (auto renderable : s_Instance->m_Renderables) {
				if (dynamic_cast<T*>(renderable)) {
					return static_cast<T*>(renderable);
				}
			}
			return nullptr;
		}

	private:
		static inline PanelManager* s_Instance;
		std::vector<GUIRenderable*> m_Renderables;
	};
}

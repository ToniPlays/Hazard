#pragma once

#include "UtilityCore.h"
#include "Event.h"

#include "GUIRenderable.h"

namespace UI 
{
	class PanelManager {
	public:

		PanelManager();

		void Render();
        bool OnEvent(Event& e) { return false; };

		template<typename T>
		static T& AddRenderable() {
			T* renderable = new T();
			s_Instance->m_Renderables.push_back(renderable);
			return *renderable;
		}

	private:
		static inline PanelManager* s_Instance;
		std::vector<GUIRenderable*> m_Renderables;
	};
}

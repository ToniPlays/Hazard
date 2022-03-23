#pragma once

#include "Ref.h"
#include <glm/glm.hpp>
#include "entt.hpp"

namespace HazardUI 
{
	class UIElement;

	class Canvas : public RefCount {
		friend class UIElement;
	public:
		Canvas();
		~Canvas();

		UIElement CreateElement(const std::string& name);
		void SetCanvasResolution(float w, float h);

		glm::vec2 GetCanvasResolution() { return m_CanvasSize; };
		template<typename... T>
		auto GetElementsWith()
		{
			return m_Registry.view<T...>();
		}


	private:
		entt::registry m_Registry = entt::registry();
		glm::vec2 m_CanvasSize;
	};
}
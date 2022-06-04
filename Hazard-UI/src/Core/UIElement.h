#pragma once

#include "Components/Components.h"
#include "Ref.h"
#include "Canvas.h"
#include "entt.hpp"
#include "Color.h"

namespace HazardUI 
{
	//This a UI element

	class UIElement {
	public:

		UIElement() {};
		UIElement(entt::entity handle, Ref<Canvas> canvas);
		~UIElement() {};

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args) {
			return m_Canvas->m_Registry.emplace<T>(*this, std::forward<Args>(args)...);
		}
		template<typename T>
		T& GetComponent() { 
			return m_Canvas->m_Registry.get<T>(m_Handle);
		}
		
		operator entt::entity() { return m_Handle; }

		Color Color;

	private:
		entt::entity m_Handle { entt::null };
		Ref<Canvas> m_Canvas;
	};
}
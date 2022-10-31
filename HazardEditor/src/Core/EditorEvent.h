#pragma once

#include "Hazard.h"
#include "Event.h"

using namespace Hazard;

namespace Events {

	class SelectionContextChange : public Event
	{
	public:
		SelectionContextChange(std::vector<Entity> entities) : m_Entities(entities) {}

		inline std::vector<Entity> GetEntitites() { return m_Entities; }
		inline Entity GetEntity(uint32_t index = 0) { return m_Entities[index]; }
		inline uint32_t GetEntityCount() { return m_Entities.size(); }

		EVENT_CLASS_TYPE(AppEvent)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		std::vector<Entity> m_Entities;
	};
}
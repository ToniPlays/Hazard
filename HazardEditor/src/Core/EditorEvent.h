#pragma once

#include "Hazard.h"

using namespace Hazard;

namespace Events {

	class SelectionContextChange : public Event
	{
	public:
		SelectionContextChange(ECS::Entity entity) : entity(entity) {}

		inline ECS::Entity GetEntity() { return entity; }

		EVENT_CLASS_TYPE(AppEvent)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		ECS::Entity entity;
	};
}
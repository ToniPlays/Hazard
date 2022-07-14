#pragma once

#include "Hazard.h"
#include "Event.h"

using namespace Hazard;

namespace Events {

	class SelectionContextChange : public Event
	{
	public:
		SelectionContextChange(Entity entity) : entity(entity) {}

		inline Entity GetEntity() { return entity; }

		EVENT_CLASS_TYPE(AppEvent)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		Entity entity;
	};
}
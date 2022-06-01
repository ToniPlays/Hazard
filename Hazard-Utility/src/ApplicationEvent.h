#pragma once

#include "Event.h"
#include "UtilityCore.h"

namespace Hazard {

	class AppTickEvent : public Event {

	public:
		AppTickEvent() = default;

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class ModuleErrorEvent : public Event
	{
	public:
		ModuleErrorEvent() = default;

		EVENT_CLASS_TYPE(ModuleError)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}
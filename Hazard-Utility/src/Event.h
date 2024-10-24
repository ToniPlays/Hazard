#pragma once

#include "UtilityCore.h"
#include <string>
#include <functional>

#define BIND_EVENT(x) std::bind(&x, this, std::placeholders::_1)

//What type of event happened
enum class EventType {
	None = 0,
	WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved, WindowTitleBarHitTest,
	AppTick, AppUpdate, AppRender, AppEvent, AppCloseEvent,
	ModuleError, ModuleWarn,
	KeyPressed, KeyReleased, KeyTyped,
	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
	GamepadButtonPressed, GamepadButtonReleased, GamepadConnected, GamepadDisconnected
};

//Events belong to a category
enum EventCategory {
	None = 0,
	EventCategoryApplication = BIT(0),
	EventCategoryInput = BIT(1),
	EventCategoryKeyboard = BIT(2),
	EventCategoryMouse = BIT(3),
	EventCategoryMouseButton = BIT(4),
	EventCategoryGamepadAxis = BIT(5),
	EventCategoryGamepadButton = BIT(6)

};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() {return EventType::type; } \
								virtual EventType GetEventType() const override { return GetStaticType();} \
								virtual const char* GetName() const override {return #type; }
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }


//Actual event class

class Event {
	friend class EventDispathcer;
public:
	bool handled = false;

	virtual EventType GetEventType() const = 0;
	virtual const char* GetName() const = 0;
	virtual int GetCategoryFlags() const = 0;
	virtual std::string ToString() const { return GetName(); }

	inline bool IsInCategory(EventCategory category) {
		return GetCategoryFlags() & category;
	}
};


//Event dispatcher handles events
class EventDispatcher 
{
public:
	EventDispatcher(Event& event) : m_Event(event) {}

	//Dispath the event for layer
	template<typename T, typename F>
	bool Dispatch(const F& eventMethod) 
	{
		if (m_Event.GetEventType() == T::GetStaticType()) 
		{
			m_Event.handled = eventMethod(static_cast<T&>(m_Event));
		}
		return m_Event.handled;
	}
private:
	Event& m_Event;
};

inline std::ostream& operator<<(std::ostream& os, const Event& e) 
{
	return os << e.ToString();
}

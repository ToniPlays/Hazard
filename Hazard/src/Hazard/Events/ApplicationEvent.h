#pragma once

#include <hzrpch.h>

namespace Hazard {

	//Contains all window events;

	class WindowResizeEvent : public Event {

	public: 
		WindowResizeEvent(unsigned int width, unsigned int height) 
			: m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "Window resized: " << m_Width << "x" << m_Height;
			return ss.str();
		}
		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		unsigned int m_Width, m_Height;
	};

	class WindowCloseEvent : public Event {

	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
	class  AppTickEvent : public Event {

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
	class WindowFocusEvent : public Event {
	public:
		WindowFocusEvent(bool _focus) : m_Focus(_focus) {};
		inline bool GetFocus() { return m_Focus; };

		EVENT_CLASS_TYPE(WindowFocus)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		bool m_Focus;
	};

	class ModuleErrorEvent : public Event
	{
	public:
		ModuleErrorEvent() = default;

		EVENT_CLASS_TYPE(ModuleError)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}
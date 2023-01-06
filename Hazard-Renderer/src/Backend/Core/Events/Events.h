#pragma once

#include "Event.h"
#include <sstream>

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
	EVENT_CLASS_TYPE(WindowResize);
	EVENT_CLASS_CATEGORY(EventCategoryApplication)

private:
	unsigned int m_Width, m_Height;
};

class WindowCloseEvent : public Event {

public:
	WindowCloseEvent() = default;

	EVENT_CLASS_TYPE(WindowClose);
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
};
class WindowFocusEvent : public Event {
public:
	WindowFocusEvent(bool _focus) : m_Focus(_focus) {};
	inline bool GetFocus() { return m_Focus; };

	EVENT_CLASS_TYPE(WindowFocus);
	EVENT_CLASS_CATEGORY(EventCategoryApplication)

private:
	bool m_Focus;
};

class WindowTitleBarHitTestEvent : public Event
{
public:
	WindowTitleBarHitTestEvent(int x, int y, int& hit)
		: m_X(x), m_Y(y), m_Hit(hit) {}

	inline int GetX() const { return m_X; }
	inline int GetY() const { return m_Y; }
	inline void SetHit(bool hit) { m_Hit = (int)hit; }

	EVENT_CLASS_TYPE(WindowTitleBarHitTest);
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
private:
	int m_X;
	int m_Y;
	int& m_Hit;
};

class KeyEvent : public Event
{
public:
	inline int GetKeyCode() const { return m_KeyCode; }

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
protected:
	KeyEvent(int keycode)
		: m_KeyCode(keycode) {}

	int m_KeyCode;
};

class KeyPressedEvent : public KeyEvent
{
public:
	KeyPressedEvent(int keycode, int repeatCount)
		: KeyEvent(keycode), m_RepeatCount(repeatCount) {}

	int GetRepeatCount() const { return m_RepeatCount; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyPressed)
private:
	int m_RepeatCount;
};

class KeyReleasedEvent : public KeyEvent
{
public:
	KeyReleasedEvent(int keycode)
		: KeyEvent(keycode) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyReleasedEvent: " << m_KeyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent
{
public:
	KeyTypedEvent(int keycode)
		: KeyEvent(keycode) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "KeyTypedEvent: " << m_KeyCode;
		return ss.str();
	}

	EVENT_CLASS_TYPE(KeyTyped)
};
class MouseMovedEvent : public Event
{
public:
	MouseMovedEvent(float x, float y)
		: m_MouseX(x), m_MouseY(y) {}

	float GetX() const { return m_MouseX; }
	float GetY() const { return m_MouseY; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseMoved);
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
	float m_MouseX, m_MouseY;
};

class MouseScrolledEvent : public Event
{
public:
	MouseScrolledEvent(float xOffset, float yOffset)
		: m_XOffset(xOffset), m_YOffset(yOffset) {}

	float GetXOffset() const { return m_XOffset; }
	float GetYOffset() const { return m_YOffset; }

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseScrolled);
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
	float m_XOffset, m_YOffset;
};

class MouseButtonEvent : public Event
{
public:
	inline int GetMouseButton() const { return m_Button; }
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    
protected:
	MouseButtonEvent(int button)
		: m_Button(button) {}

	int m_Button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
	MouseButtonPressedEvent(int button)
		: MouseButtonEvent(button) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonPressedEvent: " << m_Button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
	MouseButtonReleasedEvent(int button)
		: MouseButtonEvent(button) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "MouseButtonReleasedEvent: " << m_Button;
		return ss.str();
	}

	EVENT_CLASS_TYPE(MouseButtonReleased)
};

class GamepadEvent : public Event
{
public:
	GamepadEvent(int gamepad) : m_GamepadIndex(gamepad) {};

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "GamepadEvent: " << m_GamepadIndex;
		return ss.str();
	}
	int GetGamepadIndex() { return m_GamepadIndex; }

protected:
	int m_GamepadIndex = -1;
	EVENT_CLASS_CATEGORY(EventCategoryGamepadAxis | EventCategoryGamepadButton)
};

class GamepadConnectedEvent : public GamepadEvent
{
public:
	GamepadConnectedEvent(int gamepad)
		: GamepadEvent(gamepad) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "GamepadConnectedEvent: " << m_GamepadIndex;
		return ss.str();
	}

	EVENT_CLASS_TYPE(GamepadConnected)
};

class GamepadDisconnectedEvent : public GamepadEvent
{
public:
	GamepadDisconnectedEvent(int gamepad)
		: GamepadEvent(gamepad) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "GamepadDisconnectedEvent: " << m_GamepadIndex;
		return ss.str();
	}

	EVENT_CLASS_TYPE(GamepadConnected)
};


class GamepadAxisMovedEvent : public GamepadEvent
{
public:
	GamepadAxisMovedEvent(int gamepad, float x, float y)
		: GamepadEvent(gamepad), m_X(x), m_Y(y) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "GamepadAxisMovedEvent: " << m_X << ", " << m_Y;
		return ss.str();
	}
private:
	float m_X;
	float m_Y;
	EVENT_CLASS_TYPE(GamepadConnected)
};

class GamepadButtonEvent : public GamepadEvent
{
public:
	GamepadButtonEvent(int gamepad, int button)
		: GamepadEvent(gamepad), m_Button(button) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "GamepadButtonEvent: " << m_Button;
		return ss.str();
	}

protected:
	int m_Button;
	EVENT_CLASS_TYPE(GamepadConnected)
};
class GamepadButtonPressedEvent : public GamepadButtonEvent
{
public:
	GamepadButtonPressedEvent(int gamepad, int button)
		: GamepadButtonEvent(gamepad, button) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "GamepadButtonPressedEvent: " << m_Button;
		return ss.str();
	}
private:
	EVENT_CLASS_TYPE(GamepadConnected)
};

class GamepadButtonReleasedEvent : public GamepadButtonEvent
{
public:
	GamepadButtonReleasedEvent(int gamepad, int button)
		: GamepadButtonEvent(gamepad, button) {}

	std::string ToString() const override
	{
		std::stringstream ss;
		ss << "GamepadButtonReleasedEvent: " << m_Button;
		return ss.str();
	}
private:
	EVENT_CLASS_TYPE(GamepadConnected)
};

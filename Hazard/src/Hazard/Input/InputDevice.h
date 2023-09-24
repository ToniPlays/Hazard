#pragma once

#include "UtilityCore.h"
#include "Ref.h"

namespace Hazard
{
	enum class InputDeviceType : uint32_t
	{
		None = 0,
		Keyboard = BIT(0),
		Mouse = BIT(1),
		Gamepad = BIT(2)
	};

	static const char* InputDeviceToString(InputDeviceType device)
	{
		switch (device)
		{
			case InputDeviceType::None:		return "None";
			case InputDeviceType::Keyboard:	return "Keyboard";
			case InputDeviceType::Mouse:	return "Mouse";
			case InputDeviceType::Gamepad:	return "Gamepad";
		}
	}


	class InputDevice : public RefCount
	{
	public:
		virtual void OnConnected(uint32_t bindingPoint) = 0;
		virtual void OnDisconnected() = 0;

		virtual const std::string& GetName() const = 0;
		virtual bool IsConnected() const = 0;
		virtual InputDeviceType GetType() const = 0;

		virtual uint32_t GetBindingPoint() const = 0;
		virtual bool GetKeyState(uint32_t keycode) const = 0;
		virtual float GetAxis(uint32_t keycode, uint32_t inverseKeycode) const = 0;
	};
}
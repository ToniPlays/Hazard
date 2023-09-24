#pragma once

#include "InputDevice.h"

namespace Hazard
{
	class KeyboardInputDevice : public InputDevice
	{
	public:
		KeyboardInputDevice() = default;
		~KeyboardInputDevice() = default;

		void OnConnected(uint32_t bindingPoint) {};
		void OnDisconnected() {};

		const std::string& GetName() const { return m_Name; };
		InputDeviceType GetType() const { return InputDeviceType::Keyboard; };
		bool IsConnected() const { return true; };

		bool GetKeyState(uint32_t keycode) const;
		float GetAxis(uint32_t keycode, uint32_t inverseKeycode) const;
		uint32_t GetBindingPoint() const { return 0; };

	private:
		std::string m_Name = "Keyboard";
	};
}
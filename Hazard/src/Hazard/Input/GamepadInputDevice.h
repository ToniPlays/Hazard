#pragma once

#include "InputDevice.h"

namespace Hazard
{
	class GamepadInputDevice : public InputDevice
	{
	public:
		GamepadInputDevice() = default;
		~GamepadInputDevice() = default;

		void OnConnected(uint32_t bindingPoint) { m_BindingPoint = bindingPoint; };
		void OnDisconnected() {};

		const std::string& GetName() const;
		bool IsConnected() const;
		InputDeviceType GetType() const { return InputDeviceType::Gamepad; };

		uint32_t GetBindingPoint() const override { return 0; };
		bool GetKeyState(uint32_t keycode) const { return false; };
		float GetAxis(uint32_t keycode, uint32_t inverseKeycode) const;

	private:
		uint32_t m_BindingPoint = 0;
	};
}
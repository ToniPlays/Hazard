#pragma once

#include "InputDevice.h"

namespace Hazard
{
	class GamepadInputDevice : public InputDevice
	{
	public:
		GamepadInputDevice() = default;
		~GamepadInputDevice() = default;

		void OnConnected(uint32_t bindingPoint) override { m_BindingPoint = bindingPoint; };
		void OnDisconnected() override {};

		const std::string& GetName() const override;
		bool IsConnected() const override;
		InputDeviceType GetType() const override { return InputDeviceType::Gamepad; };

		uint32_t GetBindingPoint() const override { return 0; };
		bool GetKeyState(uint32_t keycode) const override { return false; };
		float GetAxis(uint32_t keycode, uint32_t inverseKeycode) const override;

	private:
		uint32_t m_BindingPoint = 0;
	};
}

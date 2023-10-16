#pragma once

#include "InputDevice.h"

namespace Hazard
{
	class MouseInputDevice : public InputDevice
	{
	public:
		MouseInputDevice() = default;
		~MouseInputDevice() = default;

		void OnConnected(uint32_t bindingPoint) override {};
		void OnDisconnected() override {};

		const std::string& GetName() const override { return m_Name; }
		bool IsConnected() const override { return true; }
		InputDeviceType GetType() const override { return InputDeviceType::Mouse; };

		uint32_t GetBindingPoint() const override { return 0; };
		bool GetKeyState(uint32_t keycode) const override;
		float GetAxis(uint32_t keycode, uint32_t inverseKeycode) const override;

	private:
		std::string m_Name = "Mouse";
	};
}

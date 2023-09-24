#pragma once

#include "InputDevice.h"

namespace Hazard
{
	class MouseInputDevice : public InputDevice
	{
	public:
		MouseInputDevice() = default;
		~MouseInputDevice() = default;

		void OnConnected(uint32_t bindingPoint) {};
		void OnDisconnected() {};

		const std::string& GetName() const { return m_Name; }
		bool IsConnected() const { return true; }
		InputDeviceType GetType() const { return InputDeviceType::Mouse; };

		uint32_t GetBindingPoint() const override { return 0; };
		bool GetKeyState(uint32_t keycode) const;
		float GetAxis(uint32_t keycode, uint32_t inverseKeycode) const;

	private:
		std::string m_Name = "Mouse";
	};
}
#include "GamepadInputDevice.h"
#include "Input.h"


namespace Hazard
{
	const std::string& GamepadInputDevice::GetName() const
	{
		return Input::GetGamepads()[m_BindingPoint].Name;
	}
	bool GamepadInputDevice::IsConnected() const
	{
		return Input::GetGamepads()[m_BindingPoint].Connected;
	}
	float GamepadInputDevice::GetAxis(uint32_t keycode, uint32_t inverseKeycode) const
	{
		if (Gamepad::IsAxis(keycode))
			return Input::GetAxis(m_BindingPoint, keycode);

		float positiveValue = Input::IsButtonDown(m_BindingPoint, keycode);
		float negativeValue = Input::IsButtonDown(m_BindingPoint, inverseKeycode) * -1.0f;

		return positiveValue + negativeValue;
	}
}

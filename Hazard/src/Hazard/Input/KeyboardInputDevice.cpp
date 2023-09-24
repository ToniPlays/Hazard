#include "KeyboardInputDevice.h"
#include "Backend/Input.h"

namespace Hazard
{
	bool KeyboardInputDevice::GetKeyState(uint32_t keycode) const
	{
		return Input::IsKeyDown(keycode);
	}

	float KeyboardInputDevice::GetAxis(uint32_t keycode, uint32_t inverseKeycode) const
	{
		float positive = Input::IsKeyDown(keycode);
		float negative = Input::IsKeyDown(inverseKeycode) * -1.0f;
		return positive + negative;
	}
}

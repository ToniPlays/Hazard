#include "MouseInputDevice.h"
#include "Backend/Input.h"

namespace Hazard
{
	bool MouseInputDevice::GetKeyState(uint32_t keycode) const
	{
		return Input::IsMouseButtonPressed(keycode);
	}
	float MouseInputDevice::GetAxis(uint32_t keycode, uint32_t inverseKeycode) const
	{
		if (Mouse::IsAxis(keycode))
		{
			glm::vec2 pos = Input::GetMousePos();

			if (keycode == Mouse::MovementX)
				return pos.x;
			if (keycode == Mouse::MovementY)
				return pos.y;

			return 0.0f;
		}

		float positive = Input::IsMouseButtonDown(keycode);
		float negative = Input::IsMouseButtonDown(inverseKeycode) * -1.0f;

		return positive + negative;
	}
}

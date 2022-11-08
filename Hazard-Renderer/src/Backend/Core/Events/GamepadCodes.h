#pragma once

#include <iostream>

namespace Gamepad {

	using Gamepad = uint16_t;

	enum : Gamepad
	{
		A = 0,
		B = 1,
		X = 2,
		Y = 3,
		LeftBumber = 4,
		RightBumber = 5,
		Back = 6,
		Start = 7,
		Guide = 8,
		LeftThumb = 9,
		RightThumb = 10,
		DPadUp = 11,
		DPadRight = 12,
		DPadDown = 13,
		DPadLeft = 14,
		Cross = A,
		Circle = B,
		Square = X,
		Triangle = Y,

		ButtonFirst = A,
		ButtonLast = DPadLeft,

		AxisLeft = 0,
		AxisRight = 1,
		TriggerLeft = 2,
		TriggerRight = 3,
		AxisFirst = AxisLeft,
		AxisLast = AxisRight,
	};
}
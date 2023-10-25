#pragma once

#include <iostream>
#include "UtilityCore.h"

namespace Gamepad {

	using GamepadCode = uint16_t;

	enum : GamepadCode
	{
		None = 0,
		A = 1,
		B = 2,
		X = 3,
		Y = 4,
		LeftBumber = 5,
		RightBumber = 6,
		Back = 7,
		Start = 8,
		Guide = 9,
		LeftThumb = 10,
		RightThumb = 11,
		DPadUp = 12,
		DPadRight = 13,
		DPadDown = 14,
		DPadLeft = 15,
		Cross = A,
		Circle = B,
		Square = X,
		Triangle = Y,

		ButtonFirst = A,
		ButtonLast = DPadLeft,

		AxisLeftHorizontal = 64,
		AxisLeftVertical = 65,
		AxisRightHorizontal = 66,
		AxisRightVertical = 67,
		TriggerLeft = 68,
		TriggerRight = 69,
		AxisFirst = AxisLeftHorizontal,
		AxisLast = TriggerRight,
	};
	static const char* ToString(GamepadCode code)
	{
		switch (code)
		{
			TO_STRING(None);
			TO_STRING(A);
			TO_STRING(B);
			TO_STRING(X);
			TO_STRING(Y);
			TO_STRING(LeftBumber);
			TO_STRING(RightBumber);
			TO_STRING(Back);
			TO_STRING(Start);
			TO_STRING(Guide);
			TO_STRING(LeftThumb);
			TO_STRING(RightThumb);
			TO_STRING(DPadUp);
			TO_STRING(DPadRight);
			TO_STRING(DPadDown);
			TO_STRING(DPadLeft);

			TO_STRING(AxisLeftHorizontal);
			TO_STRING(AxisLeftVertical);
			TO_STRING(AxisRightHorizontal);
			TO_STRING(AxisRightVertical);
			TO_STRING(TriggerLeft);
			TO_STRING(TriggerRight);
		}
        return "";
	}
	static bool IsAxis(GamepadCode axis)
	{
		return Gamepad::AxisFirst <= axis && axis <= Gamepad::AxisLast;
	}
}

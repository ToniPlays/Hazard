#pragma once

#include <iostream>
#include <UtilityCore.h>

namespace Mouse {

	using MouseCode = uint16_t;

	enum : MouseCode {
		None = 255,
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2,

		MovementX = 64,
		MovementY = 65,
		ScrollVertical = 66,
		ScrollHorizontal = 67
	};

	static bool IsAxis(MouseCode code)
	{
		return MovementX <= code && code <= ScrollHorizontal;
	}

	static const char* ToString(MouseCode code)
	{
		switch (code)
		{
			TO_STRING(None);
			TO_STRING(ButtonLeft);
			TO_STRING(ButtonRight);
			TO_STRING(ButtonMiddle);
			TO_STRING(Button3);
			TO_STRING(Button4);
			TO_STRING(Button5);
			TO_STRING(Button6);
			TO_STRING(Button7);
			TO_STRING(MovementX);
			TO_STRING(MovementY);
			TO_STRING(ScrollVertical);
			TO_STRING(ScrollHorizontal);
		}
	}
}
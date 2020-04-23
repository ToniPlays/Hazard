#pragma once
#include "Core/Core.h"
#include <iostream>

namespace Hazard {

	class HAZARD_API Color {

	public:
		float r, g, b, a;

		Color();
		Color(int r, int g, int b, int a);
		~Color();
		static Color fromHEX(std::string hex);
	};
}

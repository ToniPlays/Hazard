#pragma once

#include "Core/Core.h"

namespace Hazard {

	class HAZARD_API Color {
	public:
		float r, g, b, a;
	public:
		Color();
		Color(int r, int g, int b, int a);

		std::string ToString();
		static Color FromHex(std::string hex);
	};
}
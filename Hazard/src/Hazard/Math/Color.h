#pragma once
#include "Hazard/Core/Core.h"
#include <iostream>
#include "glm/glm.hpp"

namespace Hazard {

	class HAZARD_API Color {
	public:
		float r = 0, g = 0, b = 0, a = 0;
	public:
		Color();
		Color(const char* color);
		Color(int r, int g, int b, int a);
		Color(float r, float g, float b, float a);


		glm::vec4 ToGlm() { return glm::vec4(r, g, b, a); }
		static Color FromGLM(const glm::vec4 color) { return Color(color.r, color.g, color.b, color.w); }
		

		std::string ToString();

		operator std::string() {
			return this->ToString();
		}

		friend std::ostream& operator << (std::ostream& os, const Color color) {
			os << color.r << ", " << color.g << ", " << color.b << ", " << color.a;
			return os;
 		}
		static Color FromHex(std::string hex);
	};
}
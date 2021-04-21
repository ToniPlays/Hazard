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

		static Color FromGLM(const glm::vec4 color);
		static Color FromHex(std::string hex);


		friend std::ostream& operator << (std::ostream& os, const Color color) {
			os << color.r << ", " << color.g << ", " << color.b << ", " << color.a;
			return os;
 		}
		operator glm::vec4()
		{
			return glm::vec4(r, g, b, a);
		}

		operator std::string() {
			std::stringstream ss;
			ss << "R: " << this->r;
			ss << " G: " << this->g;
			ss << " B: " << this->b;
			ss << " A: " << this->a;
			return ss.str();
		}
	};
}
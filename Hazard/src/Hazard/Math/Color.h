#pragma once
#include "Hazard/Core/Core.h"
#include <iostream>
#include "glm/glm.hpp"

namespace Hazard {

	class Color {
	public:
		float r = 0, g = 0, b = 0, a = 0;
	public:
		Color();
		Color(const char* color);
		Color(int r, int g, int b, int a);
		Color(float r, float g, float b, float a);
		Color(glm::vec3 col, float a);

		static Color FromGLM(const glm::vec3 color);
		static Color FromGLM(const glm::vec4 color);
		static Color FromHex(const std::string& hex);


		friend std::ostream& operator << (std::ostream& os, const Color color) {
			os << color.r << ", " << color.g << ", " << color.b << ", " << color.a;
			return os;
 		}
		operator glm::vec4()
		{
			return glm::vec4(r, g, b, a);
		}
		static Color Black;
		static Color White;
	};
}
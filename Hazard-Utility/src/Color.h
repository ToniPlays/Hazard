#pragma once

#include <iostream>
#include <string>
#include "glm/glm.hpp"

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
	glm::vec4 ToGLM() {
		return { r, g, b, a };
	}
	operator glm::vec4()
	{
		return { r, g, b, a };
	}
	operator glm::vec3()
	{
		return { r, g, b};
	}
	Color operator = (const std::string& value) {
		return FromHex(value);
	}

public:
	static Color Black;
	static Color White;
	static Color Red;
	static Color Green;
	static Color Blue;
	static Color Yellow;
	static Color Purple;
	static Color Turqoise;
};

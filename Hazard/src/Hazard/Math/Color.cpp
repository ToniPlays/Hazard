#pragma once
#include <hzrpch.h>
#include "Color.h"

namespace Hazard
{
	Color::Color() 
	{
		this->r = 0;
		this->g = 0;
		this->b = 0;
		this->a = 1;
	}
	Color::Color(const char* color)
	{
		Color c = Color::FromHex(color);
		this->r = c.r;
		this->g = c.g;
		this->b = c.b;
		this->a = c.a;
	}
	Color::Color(int r, int g, int b, int a) {
		this->r = (float)r / 255.0f;
		this->g = (float)g / 255.0f;
		this->b = (float)b / 255.0f;
		this->a = (float)a / 255.0f;
	}
	Color::Color(float r, float g, float b, float a) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	Color Color::FromGLM(const glm::vec3 color)
	{
		return Color(color.r, color.g, color.b, 1.0f);
	}
	Color Color::FromGLM(const glm::vec4 color)
	{
		return Color(color.r, color.g, color.b, color.a);
	}
	Color Color::FromHex(std::string hex) {

		if (hex.substr(0, 1) != "#") return Color();

		int r, g, b, a = 255;
		r = std::strtoul(hex.substr(1, 2).c_str(), 0, 16);
		g = std::strtoul(hex.substr(3, 2).c_str(), 0, 16);
		b = std::strtoul(hex.substr(5, 2).c_str(), 0, 16);

		if (hex.length() == 9) a = std::strtoul(hex.substr(7, 2).c_str(), 0, 16);

		return Color(r, g, b, a);
	}
}

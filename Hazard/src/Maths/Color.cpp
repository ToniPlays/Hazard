#pragma once
#include <hzrpch.h>
#include "Color.h"

namespace Hazard 
{

	Color::Color() {

		this->r = 0;
		this->g = 0;
		this->b = 0;
		this->a = 1;
	}
	Color::Color(int r, int g, int b, int a) {
		this->r = (float)r / 255.0f;
		this->g = (float)g / 255.0f;
		this->b = (float)b / 255.0f;
		this->a = (float)a / 255.0f;
	}
	std::string Color::ToString()
	{
		std::stringstream ss;
		ss << "R: " << this->r;
		ss << " G: " << this->g;
		ss << " B: " << this->b;
		ss << " A: " << this->a;
		return ss.str();
	}
	Color Color::FromHex(std::string hex) {

		int r, g, b, a = 255;
		if (hex.substr(0, 1) != "#") {
			return Color();
		}
		r = std::strtoul(hex.substr(1, 2).c_str(), 0, 16);
		g = std::strtoul(hex.substr(3, 2).c_str(), 0, 16);
		b = std::strtoul(hex.substr(5, 2).c_str(), 0, 16);
		if (hex.length() == 9)
			a = std::strtoul(hex.substr(7, 2).c_str(), 0, 16);

		return Color(r, g, b, a);
	}
}
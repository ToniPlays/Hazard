#pragma once
#include <hzrpch.h>
#include "Color.h"

namespace Hazard {

	Color::Color()
	{
		this->r = 0.0f;
		this->g = 0.0f;
		this->b = 0.0f;
		this->a = 1.0f;
	}
	Color::Color(int r, int g, int b, int a)
	{
		this->r = (float)r / 255.0f;
		this->g = (float)g / 255.0f;
		this->b = (float)b / 255.0f;
		this->a = (float)a / 255.0f;
	}
	Color::~Color() {
	
	}
	ImVec4 Color::asImVec(Color color)
	{
		return ImVec4(color.r, color.g, color.b, color.a);
	}
	Color Color::fromHEX(std::string hex)
	{
		int r, g, b, a = 255;
		if (hex.substr(0, 1) != "#") {
			HZR_CORE_WARN("Invalid hex value {0}", hex);
			return Color();
		}
		r = std::strtoul(hex.substr(1, 2).c_str(), 0, 16);
		g = std::strtoul(hex.substr(3, 2).c_str(), 0, 16);
		b = std::strtoul(hex.substr(5, 2).c_str(), 0, 16);
		if(hex.length() == 9)
			a = std::strtoul(hex.substr(7, 2).c_str(), 0, 16);
		HZR_CORE_INFO("R: {0} G: {1} B: {2} A: {3}", r, g, b, a);

		return Color(r, g, b, a);
	}
}

#pragma once

#include "Hazard/Core/Core.h"
#include <math.h>

class HAZARD_API Math {
public:
	template<typename T, typename U>
	static T Pow(T value, U pow) {
		return std::pow(value, pow);
	}
	template<typename T>
	static T Sqrt(T value) {
		return sqrt(value);
	}
	template<typename T>
	static T Max(T a, T b) {
		return a >= b ? a : b;
	}
	template<typename T>
	static T Min(T a, T b) {
		return a <= b ? a : b;
	}
	template<typename T, typename U>
	static T Clamp(T value, U min, U max) {
		if (value <= min) return min;
		else if (value >= max) return max;
		return value;
	}
	template<typename T>
	static T Round(T value, int digits) {
		return round(value * Pow(10, digits)) / Pow(10, digits);
	}
};

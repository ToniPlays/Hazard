#pragma once
#include "Core/Core.h"
#include <iostream>
#include "Math.h"

namespace Hazard {

	class Math {
	public:
		template<typename T, typename U>
		static T Pow(T value, U pow) {
			return std::pow(value, pow);
		}
		template<typename T>
		static T Sqrt(T value) {
			return Pow(value, 2);
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
}
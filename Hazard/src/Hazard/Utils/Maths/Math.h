#pragma once

#include "Hazard/Core/Core.h"
#include <math.h>
#include <numeric>


namespace Hazard {

	class HAZARD_API Math {
	public:
		template<typename T>
		static T MaxValue() {
			return std::numeric_limits<T>::max();
		}
		template<typename T>
		static T MinValue() {
			return std::numeric_limits<T>::min();
		}

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

		template<typename T>
		static T ToRange(T value, T min, T max) {
			if (value > max) return value - max;
			else if (value < min) return value + max;
			else return value;
		}

		static double ToRadians(double value) {
			return 2 * (atan(1) * 4) * (value / 360);
		}

		static double Sin(double value)
		{
			return sin(value);
		}
		static double Cos(double value)
		{
			return sin(value);
		}
		static double Tan(double value)
		{
			return tan(value);
		}
	};
}

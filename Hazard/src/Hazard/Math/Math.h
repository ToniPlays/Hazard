#pragma once

#include <math.h>
#include <numeric>

#include <glm/glm.hpp>

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
			return (T)glm::pow(value, pow);
		}
		template<typename T>
		static T Sqrt(T value) {
			return glm::sqrt(value);
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
			float pow = Pow(10, digits);
			return glm::round(value * pow) / pow;
		}

		template<typename T>
		static T ToRange(T value, T min, T max) {
			if (value > max) return value - max;
			else if (value < min) return value + max;
			else return value;
		}

		static double ToRadians(double value) {
			return glm::radians(value);
		}

		static double Sin(double value)
		{
			return glm::sin(value);
		}
		static double Cos(double value)
		{
			return glm::cos(value);
		}
		static double Tan(double value)
		{
			return glm::tan(value);
		}

		template<typename T>
		static glm::vec2 AspectRatio(T width, T height) {
			height = (height / width) * width;
			return glm::vec2<double>(width * 0.01f, height * 0.01f);
		}
	};
}
#pragma once

#include <math.h>
#include <numeric>
#include <string>

namespace HazardUtility {
	
	template<typename T>
	inline static T MaxValue() {
		return std::numeric_limits<T>::max();
	}
	template<typename T>
	inline static T MinValue() {
		return std::numeric_limits<T>::min();
	}
	template<typename T, typename U>
	inline static T Pow(T value, U pow)
	{
		return (T)glm::pow(value, pow);
	}
	template<typename T>
	inline static T Sqrt(T value)
	{
		return glm::sqrt(value);
	}
	template<typename T>
	inline static T Floor(T value)
	{
		return glm::floor(value);
	}
	template<typename T>
	inline static T Max(T a, T b)
	{
		return a >= b ? a : b;
	}
	template<typename T>
	inline static T Min(T a, T b) {
		return a <= b ? a : b;
	}
	template<typename T, typename U>
	inline static T Clamp(T value, U min, U max)
	{
		if (value <= min) return min;
		else if (value >= max) return max;
		return value;
	}
	template<typename T>
	inline static T Round(T value, int digits)
	{
		T pow = Pow(10, digits);
		return glm::round(value * pow) / pow;
	}

	template<typename T>
	inline static T ToRange(T value, T min, T max)
	{
		if (value > max) return value - max;
		else if (value < min) return value + max;
		else return value;
	}
	template<typename T>
	static T ToDec(const std::string& value) {
		return std::strtoul(value.c_str(), 0, 16);
	}
}

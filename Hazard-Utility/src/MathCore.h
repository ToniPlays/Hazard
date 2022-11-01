#pragma once

#include <math.h>
#include <numeric>
#include <string>


#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <vector>
#include "Profiling/PerformanceProfiler.h"

namespace Math
{
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
	inline static T Min(T a, T b)
	{
		return a <= b ? a : b;
	}
	template<typename T>
	inline static T Max(T a, T b) {
		return a >= b ? a : b;
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
	static glm::mat4 ToTransformMatrix(const glm::vec3& translation) {
		return glm::translate(glm::mat4(1.0f), translation);
	}
	static glm::mat4 ToTransformMatrix(const glm::vec3& translation, const glm::quat& rotation) 
	{
		return glm::translate(glm::mat4(1.0f), translation) * glm::toMat4(rotation);
	}
	static glm::mat4 ToTransformMatrix(const glm::vec3& translation, const glm::quat& rotation, const glm::vec3& scale)
	{
		glm::mat4 t = glm::translate(glm::mat4(1.0f), translation);
		glm::mat4 s = glm::scale(glm::mat4(1.0f), scale);

		return t * glm::toMat4(rotation) * s;
	}
	static glm::vec3 GetUpDirection(const glm::quat& orientation)
	{
		return glm::rotate(orientation, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	static glm::vec3 GetRightDirection(const glm::quat& orientation)
	{
		return glm::rotate(orientation, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	static glm::vec3 GetForwardDirection(const glm::quat& orientation)
	{
		return glm::rotate(orientation, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	static glm::vec3 Scale(const glm::vec3& v, float desiredLength)
	{
		return v * desiredLength / length(v);
	}
	static bool DecomposeTransform(const glm::mat4& transform, glm::vec3& position, glm::quat& rotation, glm::vec3& scale)
	{
		HZR_TIMED_FUNCTION();
		using namespace glm;
		using T = float;

		mat4 LocalMatrix(transform);

		if (epsilonEqual(LocalMatrix[3][3], static_cast<T>(0), epsilon<T>()))
			return false;

		// Next take care of translation (easy).
		position = vec3(LocalMatrix[3]);
		LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

		vec3 Row[3];

		// Now get scale and shear.
		for (length_t i = 0; i < 3; ++i)
			for (length_t j = 0; j < 3; ++j)
				Row[i][j] = LocalMatrix[i][j];

		// Compute X scale factor and normalize first row.
		scale.x = length(Row[0]);
		Row[0] = Scale(Row[0], static_cast<T>(1));

		// Ignore shear
		//// Compute XY shear factor and make 2nd row orthogonal to 1st.
		//Skew.z = dot(Row[0], Row[1]);
		//Row[1] = detail::combine(Row[1], Row[0], static_cast<T>(1), -Skew.z);

		// Now, compute Y scale and normalize 2nd row.
		scale.y = length(Row[1]);
		Row[1] = Scale(Row[1], static_cast<T>(1));
		//Skew.z /= Scale.y;

		//// Compute XZ and YZ shears, orthogonalize 3rd row.
		//Skew.y = glm::dot(Row[0], Row[2]);
		//Row[2] = detail::combine(Row[2], Row[0], static_cast<T>(1), -Skew.y);
		//Skew.x = glm::dot(Row[1], Row[2]);
		//Row[2] = detail::combine(Row[2], Row[1], static_cast<T>(1), -Skew.x);

		// Next, get Z scale and normalize 3rd row.
		scale.z = length(Row[2]);
		Row[2] = Scale(Row[2], static_cast<T>(1));

		// Rotation as quaternion
		int i, j, k = 0;
		T root, trace = Row[0].x + Row[1].y + Row[2].z;
		if (trace > static_cast<T>(0))
		{
			root = sqrt(trace + static_cast<T>(1));
			rotation.w = static_cast<T>(0.5) * root;
			root = static_cast<T>(0.5) / root;
			rotation.x = root * (Row[1].z - Row[2].y);
			rotation.y = root * (Row[2].x - Row[0].z);
			rotation.z = root * (Row[0].y - Row[1].x);
		} // End if > 0
		else
		{
			static int Next[3] = { 1, 2, 0 };
			i = 0;
			if (Row[1].y > Row[0].x) i = 1;
			if (Row[2].z > Row[i][i]) i = 2;
			j = Next[i];
			k = Next[j];

			root = sqrt(Row[i][i] - Row[j][j] - Row[k][k] + static_cast<T>(1.0));

			rotation[i] = static_cast<T>(0.5) * root;
			root = static_cast<T>(0.5) / root;
			rotation[j] = root * (Row[i][j] + Row[j][i]);
			rotation[k] = root * (Row[i][k] + Row[k][i]);
			rotation.w = root * (Row[j][k] - Row[k][j]);
		} // End if <= 0

		return true;
	}
	static std::vector<glm::vec3> GetProjectionBounds(const glm::quat& orientation, const glm::mat4& transform, float verticalFOV, float zNear, float zFar, float aspectRatio)
	{
		glm::vec3 up = Math::GetUpDirection(orientation);
		glm::vec3 right = Math::GetRightDirection(orientation);
		glm::vec3 forward = Math::GetForwardDirection(orientation);

		verticalFOV = glm::radians(verticalFOV);
		float heightNear = 2.0f * tan(verticalFOV / 2.0f) * -zNear;
		float widthNear = heightNear * aspectRatio;

		float heightFar = 2.0f * tan(verticalFOV / 2.0f) * -zFar;
		float widthFar = heightFar * aspectRatio;

		glm::vec3 centerNear = glm::normalize(forward) * -zNear;
		glm::vec3 centerFar = glm::normalize(forward) * -zFar;

		glm::vec3 upNearHalf = (up * (heightNear / 2.0f));
		glm::vec3 upFarHalf = (up * (heightFar / 2.0f));

		glm::vec3 rightWidthNear = (right * (widthNear / 2.0f));
		glm::vec3 rightWidthFar = (right * (widthFar / 2.0f));

		std::vector<glm::vec3> result;
		result.reserve(8);

		result.push_back(centerNear + upNearHalf - rightWidthNear);		//NearTopLeft
		result.push_back(centerNear + upNearHalf + rightWidthNear);		//NearTopRight
		result.push_back(centerNear - upNearHalf + rightWidthNear);		//NearBottomRight
		result.push_back(centerNear - upNearHalf - rightWidthNear);		//NearBottomLeft

		result.push_back(centerFar + upFarHalf - rightWidthFar);		//FarTopLeft
		result.push_back(centerFar + upFarHalf + rightWidthFar);		//FarTopRight
		result.push_back(centerFar - upFarHalf + rightWidthFar);		//FarBottomRight
		result.push_back(centerFar - upFarHalf - rightWidthFar);		//FarBottomLeft

		return result;
	}
	static std::vector<glm::vec3> GetProjectionBoundsOrtho(const glm::quat& orientation, const glm::mat4& transform, float size, float zNear, float zFar, float aspectRatio)
	{
		glm::vec3 up = Math::GetUpDirection(orientation);
		glm::vec3 right = Math::GetRightDirection(orientation);
		glm::vec3 forward = Math::GetForwardDirection(orientation);

		float height = 2.0f * size / 2.0f;
		float width = height * aspectRatio;

		glm::vec3 centerNear = glm::normalize(forward) * -zNear;
		glm::vec3 centerFar = glm::normalize(forward) * -zFar;

		glm::vec3 upHalf = (up * (height / 2.0f));

		glm::vec3 rightWidth = (right * (width / 2.0f));

		std::vector<glm::vec3> result;
		result.reserve(8);

		result.push_back(centerNear + upHalf - rightWidth);		//NearTopLeft
		result.push_back(centerNear + upHalf + rightWidth);		//NearTopRight
		result.push_back(centerNear - upHalf + rightWidth);		//NearBottomRight
		result.push_back(centerNear - upHalf - rightWidth);		//NearBottomLeft

		result.push_back(centerFar + upHalf - rightWidth);		//FarTopLeft
		result.push_back(centerFar + upHalf + rightWidth);		//FarTopRight
		result.push_back(centerFar - upHalf + rightWidth);		//FarBottomRight
		result.push_back(centerFar - upHalf - rightWidth);		//FarBottomLeft

		return result;
	}
}

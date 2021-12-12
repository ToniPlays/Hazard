#pragma once

#include <math.h>
#include <numeric>

#include "Hazard/Rendering/Camera.h"

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Hazard {

	class Math {

	public:
		template<typename T>
		static T MaxValue() 
		{
			return std::numeric_limits<T>::max();
		}
		template<typename T>
		static T MinValue() 
		{
			return std::numeric_limits<T>::min();
		}

		template<typename T, typename U>
		static T Pow(T value, U pow) 
		{
			return (T)glm::pow(value, pow);
		}
		template<typename T>
		static T Sqrt(T value) 
		{
			return glm::sqrt(value);
		}
		template<typename T>
		static T Max(T a, T b) 
		{
			return a >= b ? a : b;
		}
		template<typename T>
		static T Min(T a, T b) {
			return a <= b ? a : b;
		}
		template<typename T, typename U>
		static T Clamp(T value, U min, U max) 
		{
			if (value <= min) return min;
			else if (value >= max) return max;
			return value;
		}
		template<typename T>
		static T Round(T value, int digits) 
		{
			float pow = Pow(10, digits);
			return glm::round(value * pow) / pow;
		}

		template<typename T>
		static T ToRange(T value, T min, T max) 
		{
			if (value > max) return value - max;
			else if (value < min) return value + max;
			else return value;
		}

		static double ToRadians(double value) 
		{
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
		static T ToDec(const std::string& value) {
			return std::strtoul(value.c_str(), 0, 16);
		}
		template<typename T>
		static glm::vec2 AspectRatio(T width, T height)
		{
			height = (height / width) * width;
			return glm::vec2(width * 0.01f, height * 0.01f);
		}
		static glm::mat4 ToTransformMatrix(glm::vec3 translation) {
			HZ_SCOPE_PERF("Math::ToTransformMatrix(1)", 10);
			return glm::translate(glm::mat4(1.0f), translation);
		}
		static glm::mat4 ToTransformMatrix(glm::vec3 translation, glm::vec3 rotation) {
			HZ_SCOPE_PERF("Math::ToTransformMatrix(2)", 10);
			glm::mat4 rot = glm::toMat4(glm::quat(rotation));

			return glm::translate(glm::mat4(1.0f), translation) * rot;
		}
		static glm::mat4 ToTransformMatrix(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale) 
		{
			HZ_SCOPE_PERF("Math::ToTransformMatrix(3)", 10);
			glm::mat4 rot = glm::toMat4(glm::quat(rotation));

			return glm::translate(glm::mat4(1.0f), translation)	* rot
				* glm::scale(glm::mat4(1.0f), scale);

		}
		static glm::vec3 GetUpDirection(glm::quat orientation) 
		{
			return glm::rotate(orientation, glm::vec3(0.0f, 1.0f, 0.0f));
		}
		static glm::vec3 GetRightDirection(glm::quat orientation)
		{
			return glm::rotate(orientation, glm::vec3(1.0f, 0.0f, 0.0f));
		}
		static glm::vec3 GetForwardDirection(glm::quat orientation)
		{
			return glm::rotate(orientation, glm::vec3(0.0f, 0.0f, 1.0f));
		}

		//
		static bool DecomposeTransform(const glm::mat4& transform, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale) {
			using namespace glm;
			using T = float;

			mat4 LocalMatrix(transform);

			// Normalize the matrix.
			if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
				return false;

			// First, isolate perspective.  This is the messiest.
			if (
				epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
				epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
				epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
			{
				// Clear the perspective partition
				LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
				LocalMatrix[3][3] = static_cast<T>(1);
			}

			// Next take care of translation (easy).
			position = vec3(LocalMatrix[3]);
			LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

			vec3 Row[3], Pdum3;

			// Now get scale and shear.
			for (length_t i = 0; i < 3; ++i)
				for (length_t j = 0; j < 3; ++j)
					Row[i][j] = LocalMatrix[i][j];

			// Compute X scale factor and normalize first row.
			scale.x = length(Row[0]);
			Row[0] = detail::scale(Row[0], static_cast<T>(1));
			scale.y = length(Row[1]);
			Row[1] = detail::scale(Row[1], static_cast<T>(1));
			scale.z = length(Row[2]);
			Row[2] = detail::scale(Row[2], static_cast<T>(1));

			// At this point, the matrix (in rows[]) is orthonormal.
			// Check for a coordinate system flip.  If the determinant
			// is -1, then negate the matrix and the scaling factors.
			rotation.y = asin(-Row[0][2]);
			if (cos(rotation.y) != 0) {
				rotation.x = atan2(Row[1][2], Row[2][2]);
				rotation.z = atan2(Row[0][1], Row[0][0]);
			}
			else {
				rotation.x = atan2(-Row[2][0], Row[1][1]);
				rotation.z = 0;
			}
			return true;
		}
		static std::vector<glm::vec3> GetProjectionBounds(glm::quat orientation, glm::mat4 transform, float verticalFOV, float zNear, float zFar, float aspectRatio) 
		{
			glm::vec3 up = Math::GetUpDirection(orientation);
			glm::vec3 right = Math::GetRightDirection(orientation);
			glm::vec3 forward = -Math::GetForwardDirection(orientation);

			verticalFOV = glm::radians(verticalFOV);
			float heightNear = 2.0f * tan(verticalFOV / 2.0f) * zNear;
			float widthNear = heightNear * aspectRatio;

			float heightFar = 2.0f * tan(verticalFOV / 2.0f) * zFar;
			float widthFar = heightFar * aspectRatio;

			glm::vec3 centerNear = glm::normalize(forward) * zNear;
			glm::vec3 centerFar = glm::normalize(forward) * zFar;

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
	};
}

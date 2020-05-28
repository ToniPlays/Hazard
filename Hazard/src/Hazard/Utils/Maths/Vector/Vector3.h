#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/Utils/Maths/Math.h"

namespace Hazard {
	template <typename T>
	class HAZARD_API Vector3 {
	public:
		T x, y, z;
		Vector3(T x, T y, T z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}
		Vector3() {
			this->x = 0;
			this->y = 0;
			this->z = 0;
		}
		double Length() {
			return Math::Sqrt(x * x + y * y + z * z);
		}
		void Add(Vector3 other) {
			this->x += other.x;
			this->y += other.y;
			this->z += other.z;
		}
		void Subtract(Vector3 other) {
			this->x -= other.x;
			this->y -= other.y;
			this->z -= other.z;
		}
		void Multiply(Vector3 other) {
			this->x *= other.x;
			this->y *= other.y;
			this->z *= other.z;
		}
		void Divide(Vector3 other) {
			this->x /= other.x;
			this->y /= other.y;
			this->z /= other.z;
		}
		void Normalize() {
			double len = Length();

			this->x /= len;
			this->y /= len;
			this->z /= len;
		}

		std::string ToString() {
			std::stringstream ss;
			ss << "(" << x << ", " << y << ", " << z << ")";
			return ss.str();
		}

	public:
		static Vector3 Normalized(Vector3 vector) {
			double len = vector.Length();
			return Vector3(vector.x / len, vector.y / len, vector.z / len);
		}
		static float Dot(Vector3 first, Vector3 second) {
			return first.x * second.x + first.y * second.y + first.z * second.z;
		}

		static Vector3<float> zero;
		static Vector3<float> one;
	};
	Vector3<float> Vector3<float>::zero = Vector3();
	Vector3<float> Vector3<float>::one = Vector3(1, 1, 1);
}
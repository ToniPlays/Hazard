#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/Utils/Maths/Math.h"
#include <ostream>

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
		Vector3 operator + (const Vector3<T>& vector) {
			return Vector3(x + vector.x, y + vector.y, z + vector.z);
		}
		Vector3 operator - (const Vector3<T>& vector) {
			return Vector3(x - vector.x, y - vector.y, z - vector.z);
		}
		Vector3 operator * (const Vector3<T>& vector) {
			return Vector3(x * vector.x, y * vector.y, z * vector.z);
		}
		Vector3 operator / (const Vector3<T>& vector) {
			return Vector3(x / vector.x, y / vector.y, z / vector.z);
		}

		Vector3& operator *= (const float value) {

			x *= value;
			y *= value;
			z *= value;

			return *this;
		}

		void Normalize() {
			double len = Length();

			this->x /= len;
			this->y /= len;
			this->z /= len;
		}

		void SetAll(float value) {
			this->x = value;
			this->y = value;
			this->z = value;
		}

		friend std::ostream& operator << (std::ostream& os, const Vector3<T>& vector) {
			os << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
			return os;
		}

	public:
		static Vector3 Normalized(Vector3 vector) {
			double len = vector.Length();
			return Vector3(vector.x / len, vector.y / len, vector.z / len);
		}
		static float Dot(Vector3 first, Vector3 second) {
			return first.x * second.x + first.y * second.y + first.z * second.z;
		}

		static Vector3 Invert(Vector3 vector) {
			return Vector3(-vector.x, -vector.y, -vector.z);
		}

		static Vector3<float> zero;
		static Vector3<float> one;
	};

	Vector3<float> Vector3<float>::zero = Vector3();
	Vector3<float> Vector3<float>::one = Vector3(1, 1, 1);
}
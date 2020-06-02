#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/Utils/Maths/Math.h"

namespace Hazard {
	template <typename T>
	class HAZARD_API Vector2 {
	public:
		T x, y;
		Vector2(T x, T y) {
			this->x = x;
			this->y = y;
		}
		Vector2() {
			this->x = 0;
			this->y = 0;
		}
		double Length() {
			return Math::Sqrt(x * x + y * y);
		}
		void Add(Vector2 other) {
			this->x += other.x;
			this->y += other.y;
		}
		void Subtract(Vector2 other) {
			this->x -= other.x;
			this->y -= other.y;
		}
		void Multiply(Vector2 other) {
			this->x *= other.x;
			this->y *= other.y;
		}
		void Divide(Vector2 other) {
			this->x /= other.x;
			this->y /= other.y;
		}
		void Normalize() {
			double len = Length();

			this->x /= len;
			this->y /= len;
		}

		friend std::ostream& operator << (std::ostream& os, const Vector2<T>& v) {
			os << "(" << v.x << ", " << v.y << ")";
			return os;
		}

	public:
		static Vector2 Normalized(Vector2 vector) {
			double len = vector.Length();
			return Vector2(vector.x / len, vector.y / len);
		}
		static float Dot(Vector2 first, Vector2 second) {
			return first.x * second.x + first.y * second.y;
		}
	};
}
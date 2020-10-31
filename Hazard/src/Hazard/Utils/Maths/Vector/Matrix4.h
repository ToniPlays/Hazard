#pragma once
#include "Hazard/Core/Core.h"
#include <ostream>
#include <iostream>
#include "Vector3.h"

namespace Hazard {
	class HAZARD_API Matrix4 {

	public:
		Matrix4();

		Matrix4(float init);

		void Fill(float value);
		void Set(int column, int row, float value);

		static Matrix4 InitPosition(Vector3<float> position);
		static Matrix4 InitRotation(Vector3<float> rotation);
		static Matrix4 InitScale(Vector3<float> scale);

		static Matrix4 Ortho(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);
		static Matrix4 Perspective(float fov, float aspectRatio, float zNear = 0.03f, float zFar = 1000.0f);
		static Matrix4 Inverse(Matrix4 matrix);

		static Matrix4 Multiply(const Matrix4& f, const Matrix4& s);
		static Matrix4 Multiply(const Matrix4& f, const Vector3<float>& v);
		static Vector3<float> MultiplyByVector(const Matrix4& f, const Vector3<float>& v);

		Matrix4 operator * (const Matrix4& m) {
			return Matrix4::Multiply(*this, m);
		}
		Vector3<float> operator * (const Vector3<float>& m) {
			Matrix4 mat = Matrix4::Multiply(*this, m);
			return { mat.elements[0 * 4 + 0], mat.elements[1 * 4 + 1], mat.elements[2 * 4 + 2] };
			//return Matrix4::MultiplyByVector(*this, m);
		}

		friend std::ostream& operator << (std::ostream& os, const Matrix4& m) {

			for (int x = 0; x < 4; x++) {
				os << "[";
				for (int y = 0; y < 4; y++) {
					os << m.elements[x * 4 + y];
					if (y < 3) os << ", ";
				}
				os << "]\n";
			}
			return os;
		}
		const void* data() { return elements; }

	private:
		float elements[4 * 4] = {};
	};
}
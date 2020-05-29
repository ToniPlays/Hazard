#pragma once
#include "Hazard/Core/Core.h"
#include "Transform.h"

namespace Hazard {
	class HAZARD_API Matrix4 {	
	public:
		Matrix4();

		void Identity();
		void Clear();
		std::string ToString();
		float* GetAll() { return elements; }
		void SetElement(int y, int x, float value) { elements[y * 4 + x] = value; }

		void InitTranslation(Vector3<float> position);
		void InitRotation(Vector3<float> rotation);
		void InitScale(Vector3<float> scale);
		void Mul(Matrix4 other);
		void Mul(Transform other);
		float Get(int x, int y) { return elements[y * 4 + x]; }

	public:
		static Matrix4 TransformMatrix(Transform transform);
		static Matrix4 Mul(Matrix4 first, Matrix4 second);
		static Matrix4 Ortho(float left, float right, float top, float bottom, float zNear = -1000, float zFar = 1000);
		static Matrix4 Perspec(float fov, float aspectRatio, float zNear, float zFar);
	private:
		float elements[16];
	};
}
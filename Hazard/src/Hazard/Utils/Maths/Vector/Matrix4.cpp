#pragma once
#include <hzrpch.h>
#include "Matrix4.h"

namespace Hazard {
	
	Matrix4::Matrix4()
	{
		Identity();
	}

	void Matrix4::Identity()
	{
		elements[0 * 4 + 0] = 1;
		elements[0 * 4 + 1] = 0;
		elements[0 * 4 + 2] = 0;
		elements[0 * 4 + 3] = 0;

		elements[1 * 4 + 0] = 0;
		elements[1 * 4 + 1] = 1;
		elements[1 * 4 + 2] = 0;
		elements[1 * 4 + 3] = 0;

		elements[2 * 4 + 0] = 0;
		elements[2 * 4 + 1] = 0;
		elements[2 * 4 + 2] = 1;
		elements[2 * 4 + 3] = 0;

		elements[3 * 4 + 0] = 0;
		elements[3 * 4 + 1] = 0;
		elements[3 * 4 + 2] = 0;
		elements[3 * 4 + 3] = 1;
	}

	void Matrix4::Clear()
	{
		for (int i = 0; i < 16; i++) {
			elements[i] = 0;
		}
	}

	void Matrix4::InitTranslation(Vector3<float> position)
	{
		SetElement(0, 3, position.x);
		SetElement(1, 3, position.y);
		SetElement(2, 3, position.z);
	}

	void Matrix4::InitRotation(Vector3<float> rotation)
	{

	}
	Matrix4 Matrix4::TransformMatrix(Transform transform)
	{
		Matrix4 matrix;

		matrix.InitTranslation(transform.position);
		matrix.InitRotation(transform.rotation);
		matrix.InitScale(transform.scale);
		matrix.ToString();
		return matrix;
	}

	std::string Matrix4::ToString()
	{
		std::stringstream ss;
		for (int i = 0; i < 16; i++) {
			ss << elements[i];
			if ((i + 1) % 4 == 0) ss << "\n";
			else ss << ", ";
		}
		return ss.str();
	}

	void Matrix4::InitScale(Vector3<float> scale)
	{
		SetElement(0, 0, scale.x);
		SetElement(1, 1, scale.y);
		SetElement(2, 2, scale.z);

	}

	void Matrix4::Mul(Matrix4 other)
	{
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				SetElement(i, j,
					Get(i, 0) * other.Get(0, j) +
					Get(i, 1) * other.Get(1, j) +
					Get(i, 2) * other.Get(2, j) +
					Get(i, 3) * other.Get(3, j));
			}
		}
	}

	Matrix4 Matrix4::Ortho(float left, float right, float top, float bottom, float zNear, float zFar)
	{
		Matrix4 matrix;
		matrix.Clear();

		matrix.SetElement(0, 0, 2 / (right - left));
		matrix.SetElement(1, 1, 2 / (top - bottom));
		matrix.SetElement(2, 2, -1);
		matrix.SetElement(3, 0, -(right + left) / (right - left));
		matrix.SetElement(3, 1, -(top + bottom) / (top - bottom));
		matrix.SetElement(3, 3, 1);

		return matrix;
	}
}
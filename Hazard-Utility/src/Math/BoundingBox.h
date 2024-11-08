#pragma once

#include "glm/glm.hpp"

struct BoundingBox
{
	float MinX = 0.0f, MaxX = 0.0f;
	float MinY = 0.0f, MaxY = 0.0f;
	float MinZ = 0.0f, MaxZ = 0.0f;
	
	void Encapsulate(const glm::vec3& point)
	{
		MinX = point.x < MinX ? point.x : MinX;
		MinY = point.y < MinY ? point.y : MinY;
		MinZ = point.z < MinZ ? point.z : MinZ;

		MaxX = point.x > MaxX ? point.x : MaxX;
		MaxY = point.y > MaxY ? point.y : MaxY;
		MaxZ = point.z > MaxZ ? point.z : MaxZ;
	}
	void Reset()
	{
		MinX = 0.0f;
		MinY = 0.0f;
		MinZ = 0.0f;

		MaxX = 0.0f;
		MaxY = 0.0f;
		MaxZ = 0.0f;
	}
};
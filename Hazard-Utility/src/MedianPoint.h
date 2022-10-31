#pragma once

#include "glm/glm.hpp"

struct MedianPoint
{
	glm::vec3 MedianPoint;
	uint32_t Count;

	void Reset()
	{
		MedianPoint = glm::vec3(0.0f);
		Count = 0;
	}

	void Encapsulate(const glm::vec3& point)
	{
		MedianPoint += point;
		Count++;
	}
	glm::vec3 GetMedianPoint()
	{
		return MedianPoint / (float)Count;
	}
};
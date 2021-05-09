#pragma once

#include "../Textures/CubemapTexture.h"

namespace Hazard::Rendering {

	class EnvinronmentMap : public CubemapTexture {

	public:
		EnvinronmentMap(const char* file) : CubemapTexture(file) {};
		virtual ~EnvinronmentMap() {};

		virtual void SetTexture(int side, const std::string& file) = 0;
		virtual void SetIrradiance(float irradiance) = 0;

		virtual void GenerateIrradiance() = 0;
		virtual void GeneratePreFilter() = 0;
	};
}
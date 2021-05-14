/*#pragma once

#include "../Textures/CubemapTexture.h"
#include "../Textures/Texture2D.h"

namespace Hazard::Rendering {

	class EnvinronmentMap : public CubemapTexture {

	public:
		EnvinronmentMap(const CubemapCreateInfo& info) : CubemapTexture(info) {};
		virtual ~EnvinronmentMap() {};
		 
		virtual void SetTexture(int side, const std::string& file) = 0;
		virtual Ref<Texture2D> GetTexture() = 0;
		virtual void SetIrradiance(float irradiance) = 0;

		virtual void GenerateIrradiance() = 0;
		virtual void GeneratePreFilter() = 0;
	};
}*/
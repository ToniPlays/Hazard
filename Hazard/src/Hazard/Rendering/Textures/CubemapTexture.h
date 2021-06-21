#pragma once

#include "Texture2D.h"

namespace Hazard::Rendering {

	enum CubeSide {
		Right	= 0,
		Left	= 1,
		Top		= 2,
		Bottom	= 3,
		Front	= 4,
		Back	= 5,
		All		= 6
	};
	struct CubemapSide {
		CubeSide side;
		std::string file;

		CubemapSide(CubeSide side, const std::string& file) : side(side), file(file) 
		{
		}
	};

	struct CubemapCreateInfo 
	{
		TextureDataType datatype = TextureDataType::Auto;
		uint32_t width = 0;
		uint32_t height = 0;

		std::vector<CubemapSide> sides;
	};

	struct CubemapInfo {
		TextureDataType datatype = TextureDataType::Auto;

		uint32_t width = 0;
		uint32_t height = 0;
		std::vector<Ref<Texture2D>> cubeSides;
	};

	class CubemapTexture : public Texture {

	public:
		CubemapTexture() {};
		virtual ~CubemapTexture() {};
		virtual void SetTexture(int side, const std::string& file) = 0;
		virtual CubemapInfo GetInfo() const = 0;

	};
}
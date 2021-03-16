#pragma once

#include "Texture.h"

namespace Hazard::Rendering {

	enum CubeSide {
		Right  = 0,
		Left   = 1,
		Top    = 2,
		Bottom = 3,
		Front  = 4, 
		Back   = 5
	};

	class CubemapTexture : public Texture {
	public:
		CubemapTexture(const char* file) : Texture(file) {};
		virtual ~CubemapTexture() {};

		virtual void SetTexture(int side, const std::string& file) = 0;

	};
}
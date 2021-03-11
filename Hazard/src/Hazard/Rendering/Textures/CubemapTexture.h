#pragma once

#include "Texture.h"

namespace Hazard::Rendering {

	class CubemapTexture : public Texture {
	public:
		CubemapTexture() : Texture("") {};
		virtual ~CubemapTexture() {};
	};
}
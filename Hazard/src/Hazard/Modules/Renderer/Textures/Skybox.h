#pragma once

#include "CubemapTexture.h"

namespace Hazard {
	class Skybox {
	public:
		Skybox() {};
		virtual ~Skybox() {};

		virtual void Render() = 0;
		virtual void SetCubemap(CubemapTexture* texture) = 0;
	};
}
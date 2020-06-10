#pragma once

#include "Hazard/Core/Core.h"
#include "Texture.h"

namespace Hazard {
	class HAZARD_API Texture2D : public Texture {
	public:
		Texture2D(std::string file) : Texture(file) {};
		virtual ~Texture2D() {};
	};
}
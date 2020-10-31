#pragma once

#include "Hazard/Core/Core.h"
#include "Texture.h"

namespace Hazard {
	class HAZARD_API Texture2D : public Texture {
	public:
		Texture2D(const char* file) : Texture(file) {};
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual ~Texture2D() {};
	};
}
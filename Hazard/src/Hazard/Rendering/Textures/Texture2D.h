#pragma once

#include "Hazard/Core/Core.h"
#include "Texture.h"

namespace Hazard::Rendering {

	class Texture2D : public Texture {

	public:
		Texture2D(const char* file, TextureSpecs params = TextureSpecs()) : Texture(file) {};
		virtual ~Texture2D() {};

		virtual void SetData(void* data, uint32_t size) = 0;
		virtual uint32_t GetID() const = 0;

	};
}
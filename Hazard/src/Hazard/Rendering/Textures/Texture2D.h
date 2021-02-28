#pragma once

#include "Hazard/Core/Core.h"
#include "Texture.h"

namespace Hazard::Rendering {
	class HAZARD_API Texture2D : public Texture {
	public:
		Texture2D() {};

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind(uint32_t slot = 0) const = 0;

		virtual uint32_t GetID() const = 0;

		virtual ~Texture2D() {};
	};
}
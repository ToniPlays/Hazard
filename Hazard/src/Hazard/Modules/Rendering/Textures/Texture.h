#pragma once

#include "Hazard/Core/Core.h"
namespace Hazard {
	class HAZARD_API Texture {
	public:

		virtual ~Texture() {};
		virtual unsigned int GetWidth() const { return width; };
		virtual unsigned int GetHeight() const { return height; };

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind(uint32_t slot = 0) const = 0;

		virtual uint32_t GetID() const = 0;

	protected:
		unsigned int width, height;

	};
}
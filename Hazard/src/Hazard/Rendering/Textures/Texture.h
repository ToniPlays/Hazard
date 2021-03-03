#pragma once

#include "Hazard/Core/Core.h"
#include <iostream>

namespace Hazard::Rendering {
	class HAZARD_API Texture {
	public:
		Texture() {};

		virtual ~Texture() {};
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind(uint32_t slot = 0) const = 0;
		virtual void SetData(void* data, uint32_t size) = 0;

		virtual uint32_t GetID() const = 0;

	};
}
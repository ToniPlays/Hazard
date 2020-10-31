#pragma once

#include "Hazard/Core/Core.h"
#include <iostream>

namespace Hazard {
	class HAZARD_API Texture {
	public:
		Texture(const char* file) : path(file) {};

		virtual ~Texture() {};
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind(uint32_t slot = 0) const = 0;

		virtual uint32_t GetID() const = 0;
		virtual const char* GetPath() { return path; }

	protected:
		const char* path;
	};
}
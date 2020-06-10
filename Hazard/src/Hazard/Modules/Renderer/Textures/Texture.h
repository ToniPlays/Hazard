#pragma once

#include "Hazard/Core/Core.h"
#include <iostream>

namespace Hazard {
	class HAZARD_API Texture {
	public:
		Texture(std::string file) : path(file) {};

		virtual ~Texture() {};
		virtual unsigned int GetWidth() const { return width; };
		virtual unsigned int GetHeight() const { return height; };

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind(uint32_t slot = 0) const = 0;

		virtual uint32_t GetID() const = 0;
		virtual std::string GetPath() { return path; }

	protected:
		unsigned int width = 0, height = 0;
		std::string path;
	};
}
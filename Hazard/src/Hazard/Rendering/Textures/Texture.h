#pragma once

#include "Hazard/Core/Core.h"
#include <iostream>

namespace Hazard::Rendering {

	struct TextureParam {
		uint8_t samples = 2;
	};

	class HAZARD_API Texture {
	public:
		Texture(const char* file) : file(file) {};

		virtual ~Texture() {};
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind(uint32_t slot = 0) const = 0;

		virtual uint32_t GetID() const = 0;
		std::string& GetFile() { return file; };
	protected:
		std::string file = "";
	};
}
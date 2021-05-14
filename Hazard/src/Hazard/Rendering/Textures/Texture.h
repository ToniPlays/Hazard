#pragma once

#include "Hazard/Core/Core.h"
#include <iostream>

namespace Hazard::Rendering {

	enum TextureDataType { Auto = 0, RGB, RGBA, HDR, HDRA };

	class Texture : public RefCount {
	public:
		Texture(const std::string& file) : m_File(file) {};
		virtual ~Texture();

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind(uint32_t slot = 0) const = 0;

		virtual uint32_t GetID() const = 0;
		std::string& GetFile() { return m_File; };

	protected:
		std::string m_File = "";
	};
}
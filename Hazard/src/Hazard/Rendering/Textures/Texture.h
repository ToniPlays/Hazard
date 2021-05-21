#pragma once

#include "Hazard/Core/Core.h"
#include <iostream>

namespace Hazard::Rendering {

	enum TextureDataType { Auto = 0, RGB, RGBA, HDR, HDRA };

	struct TextureInfo
	{
		std::string file;
		uint32_t width = 0;
		uint32_t height = 0;
		TextureDataType datatype;

		bool hasTransparency = false;
	};

	class Texture : public RefCount {
	public:
		virtual ~Texture();

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind(uint32_t slot = 0) const = 0;

		virtual uint32_t GetID() const = 0;
		virtual TextureInfo GetData() const = 0;
	};
}
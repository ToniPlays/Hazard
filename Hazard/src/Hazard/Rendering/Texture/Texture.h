#pragma once

#include "Hazard/Assets/Asset.h"

namespace Hazard::Rendering 
{
	enum TextureWrap { Repeat = 0, RepeatMirror, ClampBorder, ClampEdge };
	enum FilterMode { Linear = 0, Nearest, LinearMip, NearestMip };
	enum TextureDataType { Auto = 0, RGB, RGBA, HRD, HDRA };

	struct TextureFilter
	{
		FilterMode MinFilter;
		FilterMode MagFilter;
	};

	class Texture : public Asset {
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual void* GetID() const = 0;
	};
}
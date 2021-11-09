#pragma once
#include "Hazard/Core/Buffer.h"
#include "Hazard/Assets/Asset.h"

namespace Hazard::Rendering 
{
	enum class ImageFormat
	{
		None = 0,
		RED32F,
		RGB,
		RGBA,
		RGBA16F,
		RGBA32F,
		RG16F,
		RG32F,
		SRGB,

		DEPTH32F,
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8,
	};
	enum ImageWrap { Repeat = 0, RepeatMirror, ClampBorder, ClampEdge };
	enum FilterMode { Linear = 0, Nearest, LinearMip, NearestMip };
	enum ImageDataType { Auto = 0, RGB, RGBA, HRD, HDRA };
	enum ImageUsage { None = 0, Texture, Attachment, Storage };

	struct TextureFilter
	{
		FilterMode MinFilter;
		FilterMode MagFilter;

		TextureFilter() = default;
		TextureFilter(FilterMode minFilter, FilterMode magFilter) : MinFilter(minFilter), MagFilter(magFilter) {}
	};


	class Image : public Asset {
	public:
		virtual ~Image() = default;

		virtual void Invalidate() = 0;
		virtual void Release() = 0;

		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;

		virtual float GetAspectRatio() = 0;
		virtual Buffer GetBuffer() const = 0;
		virtual Buffer& GetBuffer() = 0;

		virtual void* GetID() const = 0;
	};
}
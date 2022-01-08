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
	enum class ImageWrap { Repeat = 0, RepeatMirror, ClampBorder, ClampEdge };
	enum class FilterMode { Linear = 0, Nearest, LinearMip, NearestMip };
	enum class ImageDataType { Auto = 0, RGB, RGBA, HRD, HDRA };
	enum class ImageUsage { None = 0, Texture, Attachment, Storage };

	struct TextureFilter
	{
		FilterMode MinFilter = FilterMode::Linear;
		FilterMode MagFilter = FilterMode::Linear;

		TextureFilter() = default;
		TextureFilter(FilterMode minFilter, FilterMode magFilter) : MinFilter(minFilter), MagFilter(magFilter) {}
	};


	class Image : public RuntimeResource {
	public:
		virtual ~Image() = default;

		virtual void Invalidate() = 0;
		virtual void Release() = 0;

		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;

		virtual float GetAspectRatio() = 0;
		virtual Buffer GetBuffer() const = 0;
		virtual const Buffer& GetBuffer() = 0;
	};
}
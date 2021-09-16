#pragma once
#include "Hazard/Core/Core.h"
#include "../Textures/Texture.h"

namespace Hazard::Rendering {

	enum class FrameBufferTextureFormat 
	{
		None = 0,

		RGBA8,
		RGBA16F,
		RGBA32F,
		RG32F,

		DEPTH32F,
		DEPTH32_STENCIL8,
		Depth = DEPTH32_STENCIL8

	};


	struct FrameBufferAttachment 
	{
		FrameBufferTextureFormat format = FrameBufferTextureFormat::None;

		FrameBufferAttachment() = default;
		FrameBufferAttachment(FrameBufferTextureFormat format) : format(format) {}
	};

	struct FrameBufferCreateInfo {

		const char* name = "RenderTexture";
		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t samples = 1;


		std::vector<FrameBufferAttachment> attachments;
	};
	struct FrameBufferInfo {
		uint32_t width = 0;
		uint32_t height = 0;
		uint32_t samples = 1;

		std::vector<FrameBufferAttachment> colorAttachments;
		FrameBufferAttachment depthAttachment;
	};

	class FrameBuffer : public RefCount {
	public:

		~FrameBuffer() = default;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind(uint32_t slot = 0) const = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual uint32_t GetColorID(uint32_t index = 0) const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;
		virtual FrameBufferInfo GetInfo() const = 0;
	};
}
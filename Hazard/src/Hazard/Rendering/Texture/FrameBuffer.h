#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/Assets/Asset.h"

namespace Hazard::Rendering
{
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
		void* userData = nullptr;

		FrameBufferAttachment() = default;
		FrameBufferAttachment(FrameBufferTextureFormat format) : format(format) {}

		bool IsDepth() {
			return format > FrameBufferTextureFormat::RG32F;
		};
	};

	struct FrameBufferCreateInfo
	{
		uint32_t Width = 1024, Height = 1024;
		uint32_t AttachmentCount = 0;
		std::vector<FrameBufferAttachment> Attachments;
		bool SwapChainTarger = false;
		void* userData = nullptr;
	};

	class FrameBuffer : public RefCount
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void* GetID() const = 0;

		static Ref<FrameBuffer> Create(FrameBufferCreateInfo* info);
	};
}

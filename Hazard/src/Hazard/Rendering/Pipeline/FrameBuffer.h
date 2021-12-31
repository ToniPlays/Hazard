#pragma once

#include "Hazard/Core/Core.h"
#include "glm/glm.hpp"
#include "../Image/Image2D.h"

namespace Hazard::Rendering {
	class FrameBuffer;

	struct FrameBufferAttachment
	{
		ImageFormat Format = ImageFormat::None;

		FrameBufferAttachment() = default;
		FrameBufferAttachment(ImageFormat format) : Format(format) {}

		bool IsDepth() {
			return Format > ImageFormat::SRGB;
		}
	};

	struct FrameBufferCreateInfo
	{
		uint32_t Width = 0, Height = 0;
		uint32_t Samples = 1;
		uint32_t AttachmentCount = 0;
		glm::vec4 ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		std::vector<FrameBufferAttachment> Attachments;
		Ref<FrameBuffer> pFrameBuffer = nullptr;
		bool SwapChainTarget = false;
		bool ClearOnLoad = false;
		std::string DebugName = "";
	};
	struct FrameBufferSpecification {
		uint32_t Width, Height;
		uint32_t Samples;
		uint32_t AttachmentCount;
		glm::vec4 ClearColor;
		std::vector<FrameBufferAttachment> Attachments;
		Ref<FrameBuffer> pFrameBuffer = nullptr;
		bool SwapChainTarget;
		bool ClearOnLoad;
		std::string DebugName;
	};

	class FrameBuffer : public RefCount {
	public:
		virtual ~FrameBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Resize(uint32_t width, uint32_t height, bool force = false) = 0;
		virtual void BindTexture(uint32_t attachmentIndex = 0, uint32_t slot = 0) const = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual Ref<Image2D> GetImage() const = 0;
		virtual FrameBufferSpecification& GetSpecification() = 0;

		static Ref<FrameBuffer> Create(FrameBufferCreateInfo* info);
	};
}
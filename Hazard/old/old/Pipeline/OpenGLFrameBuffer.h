#pragma once

#include "Hazard/Rendering/Pipeline/FrameBuffer.h"
#include "glad/glad.h"

namespace Hazard::Rendering::OpenGL {

	class OpenGLFrameBuffer : public FrameBuffer {
	public:

		OpenGLFrameBuffer(const FrameBufferCreateInfo& info);
		virtual ~OpenGLFrameBuffer();


		void Bind(uint32_t slot = 0) const override;
		void Unbind(uint32_t slot = 0) const override;
		void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetWidth() const override { return 0; }
		virtual uint32_t GetHeight() const override { return 0; }

		uint32_t GetColorID(uint32_t index) const { return m_ColorIDs[index]; }

		FrameBufferInfo GetInfo() const override { return m_Info; }
		uint32_t GetID() const { return m_RendererID; };
	private:

		void Invalidate();
		FrameBufferInfo m_Info;

		GLuint m_RendererID = 0;
		std::vector<uint32_t> m_ColorIDs;
		uint32_t m_DepthID;
	};
}
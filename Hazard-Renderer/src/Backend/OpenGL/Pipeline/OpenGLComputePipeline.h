#pragma once

#include "Backend/Core/Pipeline/ComputePipeline.h"
#include "Backend/Core/Pipeline/Shader.h"

namespace HazardRenderer::OpenGL 
{
	class OpenGLComputePipeline : public ComputePipeline {
	public:
		OpenGLComputePipeline(ComputePipelineCreateInfo* createInfo);
		~OpenGLComputePipeline();

		void Bind(Ref<RenderCommandBuffer> commandBuffer) override;
		void Execute(Ref<RenderCommandBuffer> commandBuffer) override;

		Ref<Shader> GetShader() override { return m_Shader; }

	private:
		Ref<Shader> m_Shader;
	};
}
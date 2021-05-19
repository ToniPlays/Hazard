#pragma once

#include "Hazard/Rendering/Pipeline/GraphicsPipeline.h"

namespace Hazard::Rendering::OpenGL 
{
	class OpenGLGraphicsPipeline : public GraphicsPipeline {
	public:
		OpenGLGraphicsPipeline(GraphicsPipelineCreateInfo info);
		~OpenGLGraphicsPipeline();

		void Bind() override;

		Ref<Shader> GetShader() override { return m_Info.pipelineShader; }
	private:
		GraphicsPipelineInfo m_Info;
	};
}
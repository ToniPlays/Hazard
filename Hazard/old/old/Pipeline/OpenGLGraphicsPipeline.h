#pragma once

namespace Hazard::Rendering::OpenGL 
{
	class OpenGLGraphicsPipeline : public GraphicsPipeline {
	public:
		OpenGLGraphicsPipeline(GraphicsPipelineCreateInfo info);
		~OpenGLGraphicsPipeline();

		void Bind() override;

		Ref<Shader> GetShader() override { return m_Info.pipelineShader; }
		GraphicsPipelineInfo GetInfo() { return m_Info; }
	private:
		GraphicsPipelineInfo m_Info;
	};
}
#pragma once

#include "Hazard/Rendering/Pipeline.h"
#include "OpenGLShader.h"

namespace Hazard::Rendering::OpenGL 
{
	class OpenGLPipeline : public Pipeline {
	public:
		OpenGLPipeline(const PipelineSpecification& specs);
		virtual ~OpenGLPipeline();

		virtual PipelineSpecification GetSpecifications() { return m_Specs; }
		virtual const PipelineSpecification GetSpecifications() const { return m_Specs; }

		virtual void Invalidate() override;
		virtual void Bind() override;

	private:
		PipelineSpecification m_Specs;
		Shader* m_Shader;
	};
}
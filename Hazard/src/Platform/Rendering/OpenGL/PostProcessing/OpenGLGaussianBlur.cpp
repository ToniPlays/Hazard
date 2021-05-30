#pragma once

#include <hzrpch.h>
#include "OpenGLGaussianBlur.h"
#include "Hazard/Rendering/RenderCommand.h"
#include "../Pipeline/OpenGLFrameBuffer.h"

namespace Hazard::Rendering::OpenGL 
{
    OpenGLGaussianBlur::OpenGLGaussianBlur(GaussianBlurCreateInfo* info)
    {
        use = info->use;

        FrameBufferCreateInfo frameBufferInfo;
        frameBufferInfo.name = "GaussianBlurr";
        frameBufferInfo.width = 1920;
        frameBufferInfo.height = 1080;
        frameBufferInfo.attachments = { FrameBufferTextureFormat::RGBA8 };

        m_Target = new OpenGLFrameBuffer(frameBufferInfo);
        m_BackBuffer = new OpenGLFrameBuffer(frameBufferInfo);

        PipelineShaderStage stages[2];

        stages[0].shaderFileName = "horizontalGaussian_vert.glsl";
        stages[0].stage = ShaderType::VertexShader;
        stages[0].fileType = ShaderFileType::Source;

        stages[1].shaderFileName = "gaussian_blurr_frag.glsl";
        stages[1].stage = ShaderType::FragmentShader;
        stages[1].fileType = ShaderFileType::Source;

        PipelineRasterizer rasterizer = {};
        rasterizer.cullFace = CullFace::BackFace;
        rasterizer.depthFunc = DepthFunc::Less;

        PipelineViewport viewport;
        viewport.size = { 1280, 720 };

        GraphicsPipelineCreateInfo pipelineInfo;
        pipelineInfo.shaderPipelineName = "HorizontalGaussian";
        pipelineInfo.viewport = &viewport;
        pipelineInfo.rasterizer = &rasterizer;
        pipelineInfo.stageCount = 2;
        pipelineInfo.stages = stages;

        m_horizontalPipeline = RenderUtils::Create<GraphicsPipeline>(pipelineInfo);
        m_horizontalPipeline->GetShader()->SetUniformInt("sampleTexture", 0);

        stages[0].shaderFileName = "verticalGaussian_vert.glsl";
        pipelineInfo.shaderPipelineName = "VerticalGaussian";

        m_verticalPipeline = RenderUtils::Create<GraphicsPipeline>(pipelineInfo);
        m_verticalPipeline->GetShader()->Bind();
        m_verticalPipeline->GetShader()->SetUniformInt("sampleTexture", 0);
        
    }
    OpenGLGaussianBlur::~OpenGLGaussianBlur()
    {

    }
    FrameBuffer* OpenGLGaussianBlur::Process(FrameBuffer* source, VertexArray* vao)
    {
        glBindTexture(GL_TEXTURE_2D, source->GetColorID(0));
        m_BackBuffer->Bind();
        RenderContextCommand::ClearFrame({ 0, 0, 0, 1.0 });
        m_horizontalPipeline->GetShader()->Bind();

        RenderCommand::DrawIndexed(vao, vao->GetIndexBuffer()->GetInfo().count);
        glBindTexture(GL_TEXTURE_2D, m_BackBuffer->GetColorID(0));

        m_Target->Bind();

        m_verticalPipeline->GetShader()->Bind();
        RenderCommand::DrawIndexed(vao, vao->GetIndexBuffer()->GetInfo().count);

        return m_Target;

    }
    void OpenGLGaussianBlur::UpdateData(PostProcessData& data)
    {

    }
}

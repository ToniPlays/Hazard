#pragma once

#include <hzrpch.h>
#include "Mesh.h"
#include "../RenderCommand.h"
#include "MeshFactory.h"
#include "Hazard/Rendering/Pipeline/Pipeline.h"

namespace Hazard::Rendering {

    Mesh::Mesh(const std::string& file, std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices) : m_Filename(file), m_Vertices(vertices), m_Indices(indices)
    {
        GeneratePipeline();
    }
    Mesh::~Mesh()
    {
        
    }
    void Mesh::GeneratePipeline()
    {
        VertexBufferCreateInfo vertexInfo = {};
        vertexInfo.Size = m_Vertices.size() * sizeof(Vertex3D);
        vertexInfo.Data = m_Vertices.data();
        vertexInfo.Usage = BufferUsage::StaticDraw;

        IndexBufferCreateInfo indexInfo = {};
        indexInfo.Size = m_Indices.size();
        indexInfo.Data = m_Indices.data();
        indexInfo.Usage = BufferUsage::StaticDraw;

        FrameBufferCreateInfo frameBufferInfo = {};
        frameBufferInfo.SwapChainTarget = false;
        frameBufferInfo.AttachmentCount = 2;
        frameBufferInfo.Attachments = { {ImageFormat::RGBA }, {ImageFormat::Depth } };
        frameBufferInfo.ClearOnLoad = true;
        frameBufferInfo.ClearColor = Color::White;
        frameBufferInfo.DebugName = "Mesh3D";
        frameBufferInfo.Width = 1920;
        frameBufferInfo.Height = 1080;

        Ref<FrameBuffer> frameBuffer = FrameBuffer::Create(&frameBufferInfo);

        RenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.pTargetFrameBuffer = frameBuffer;
        renderPassInfo.DebugName = "Mesh3D";

        Ref<RenderPass> renderPass = RenderPass::Create(&renderPassInfo);

        PipelineSpecification spec = {};
        spec.Usage = PipelineUsage::GraphicsBit;
        spec.DrawType = DrawType::Fill;
        spec.LineWidth = 1.0f;
        spec.pVertexBuffer = &vertexInfo;
        spec.pIndexBuffer = &indexInfo;
        spec.ShaderPath = "Shaders/pbr.glsl";
        spec.RenderPass = renderPass;

        m_Pipeline = Pipeline::Create(spec);
    }
}
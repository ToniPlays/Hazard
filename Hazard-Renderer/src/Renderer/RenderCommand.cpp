
#include "RenderCommand.h"
#include "Backend/Core/Window.h"

namespace HazardRenderer 
{
    void RenderCommand::BeginFrame() 
    {
        s_DrawList.Geometry.clear();
        s_DrawList.Quads = 0;
    }
    void RenderCommand::BeginWorld(WorldSettings& settings)
    {
        s_DrawList.Settings = settings;
    }
    //void RenderCommand::DrawQuad(Ref<Pipeline> pipeline, glm::mat4 transform, glm::vec4 color)
    //{
    //    DrawQuad(pipeline, transform, color);
    //}
    //void RenderCommand::DrawQuad(Ref<Pipeline> pipeline, glm::mat4 transform, Ref<Texture2D> texture)
    //{
    //   DrawQuad(pipeline, transform, glm::vec4(1.0f), texture);
    //}
    void RenderCommand::DrawQuad(Ref<Pipeline>& pipeline, const glm::mat4& transform, const glm::vec4& color)
    {
        PipelineRenderable& renderable = s_DrawList.Geometry[pipeline->GetHandle()];
        renderable.Pipeline = pipeline;
        InstancedMesh& mesh = renderable.MeshInstances[m_QuadMesh->GetHandle()];
        mesh.RawMesh = m_QuadMesh;

        auto& data = mesh.Transforms.emplace_back();
       
        data.MRow[0] = { transform[0][0], transform[1][0], transform[2][0], transform[3][0] };
        data.MRow[1] = { transform[0][1], transform[1][1], transform[2][1], transform[3][1] };
        data.MRow[2] = { transform[0][2], transform[1][2], transform[2][2], transform[3][2] };

        s_DrawList.Quads++;
    }
    void RenderCommand::Flush()
    {
        s_Context->BeginRenderPass(m_CommandBuffer, s_DrawList.Settings.TargetRenderPass);
        for (auto [uuid, renderable] : s_DrawList.Geometry) {
            Ref<Pipeline> pipeline = renderable.Pipeline;

            pipeline->Bind(m_CommandBuffer);
            for (auto [meshUUID, rawMesh] : renderable.MeshInstances) {
                
                Ref<VertexBuffer> buffer = rawMesh.RawMesh->GetVertexBuffer();
                Ref<IndexBuffer> ibo = rawMesh.RawMesh->GetIndexBuffer();

                buffer->Bind(m_CommandBuffer);
                ibo->Bind(m_CommandBuffer);

                m_InstanceBuffer->SetData(rawMesh.Transforms.data(), rawMesh.Transforms.size() * m_InstanceBuffer->GetLayout().GetStride());
                pipeline->DrawInstanced(m_CommandBuffer, ibo->GetCount(), rawMesh.Transforms.size());
            }
        }
        s_Context->EndRenderPass(m_CommandBuffer);
    }
    void RenderCommand::RegisterPipelineDependency(Ref<Pipeline> pipeline)
    {

    }
    void RenderCommand::Init(Window* window)
    {
        s_Context = window->GetContext();

        uint32_t size = 1000 * 1000;

        float vertices[] =
        {
            -0.05f, -0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
             0.05f, -0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
             0.05f,  0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            -0.05f,  0.05f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f
        };
        uint32_t indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        BufferLayout instanceLayout = { { "a_Position", ShaderDataType::Float3 }, { "a_Color", ShaderDataType::Float4 } };
        BufferLayout perInstanceLayout = { { { "a_MRow0", ShaderDataType::Float4 }, { "a_MRow1", ShaderDataType::Float4 }, { "a_MRow2", ShaderDataType::Float4 } }, PerInstance };

        VertexBufferCreateInfo vbo = {};
        vbo.DebugName = "InstancedQuad";
        vbo.IsShared = false;
        vbo.Layout = &instanceLayout;
        vbo.Size = sizeof(vertices);
        vbo.Data = vertices;

        Ref<VertexBuffer> quadBuffer = VertexBuffer::Create(&vbo);

        VertexBufferCreateInfo instanceVBO = {};
        instanceVBO.DebugName = "InstanceVBO";
        instanceVBO.Usage = BufferUsage::DynamicDraw;
        instanceVBO.Layout = &perInstanceLayout;
        instanceVBO.Size = size * perInstanceLayout.GetStride();
        instanceVBO.pTargetBuffer = quadBuffer;

        IndexBufferCreateInfo ibo = {};
        ibo.DebugName = "TriangleIBO";
        ibo.Usage = BufferUsage::StaticDraw;
        ibo.Size = sizeof(indices);
        ibo.Data = indices;

        m_InstanceBuffer = VertexBuffer::Create(&instanceVBO);
        Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(&ibo);

        m_QuadMesh = Ref<RawMesh>::Create(quadBuffer, indexBuffer);
        m_CommandBuffer = RenderCommandBuffer::CreateFromSwapchain();
    }
}
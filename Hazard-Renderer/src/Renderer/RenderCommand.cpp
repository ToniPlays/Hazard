
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
        InstancedMesh& mesh = renderable.MeshInstances[s_Resources.QuadMesh->GetHandle()];
        mesh.InstanceCount++;
        mesh.RawMesh = s_Resources.QuadMesh;

        auto& data = mesh.Transforms.emplace_back();
        data.Color = color;
        data.MRow[0] = { transform[0][0], transform[1][0], transform[2][0], transform[3][0] };
        data.MRow[1] = { transform[0][1], transform[1][1], transform[2][1], transform[3][1] };
        data.MRow[2] = { transform[0][2], transform[1][2], transform[2][2], transform[3][2] };

        s_DrawList.Quads++;
    }
    void RenderCommand::Flush()
    {
        //s_Context->BeginRenderPass(m_CommandBuffer, s_DrawList.Settings.TargetRenderPass);

        for (auto& [uuid, renderable] : s_DrawList.Geometry) {
            Ref<Pipeline>& pipeline = renderable.Pipeline;

            pipeline->Bind(m_CommandBuffer);
            for (auto& [meshUUID, rawMesh] : renderable.MeshInstances) {
                
                auto& data = rawMesh.Transforms;
                Ref<VertexBuffer> buffer = rawMesh.RawMesh->GetVertexBuffer();
                Ref<IndexBuffer> ibo = rawMesh.RawMesh->GetIndexBuffer();

                ibo->Bind(m_CommandBuffer);
                buffer->Bind(m_CommandBuffer, 0);
                s_Resources.InstanceBuffer->Bind(m_CommandBuffer, 1);

                s_Resources.InstanceBuffer->SetData(data.data(), (uint32_t)data.size() * sizeof(TransformData));
                pipeline->DrawInstanced(m_CommandBuffer, ibo->GetCount(), rawMesh.InstanceCount);
            }
        }
       //s_Context->EndRenderPass(m_CommandBuffer);
    }
    void RenderCommand::RegisterPipelineDependency(Ref<Pipeline> pipeline)
    {

    }
    void RenderCommand::Init(Window* window)
    {
        s_Context = window->GetContext();
#if 0
        uint32_t size = 500;

        float vertices[] =
        {
            //Vec3 pos,        vec4 color
            -0.5f, -0.5f, 0.0f,// 1.0f, 1.0f, 1.0f, 1.0f,
             0.5f, -0.5f, 0.0f,// 1.0f, 1.0f, 1.0f, 1.0f,
             0.5f,  0.5f, 0.0f,// 1.0f, 1.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f // 1.0f, 1.0f, 1.0f, 1.0f
        };
        uint32_t indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        BufferLayout meshLayout = {
            { "a_Position", ShaderDataType::Float3                      }, 
            { "a_Color",    ShaderDataType::Float4, false, PerInstance  }, 
            { "a_MRo0",     ShaderDataType::Float4, false, PerInstance  }, 
            { "a_MRo1",     ShaderDataType::Float4, false, PerInstance  },
            { "a_MRo2",     ShaderDataType::Float4, false, PerInstance  }     
        };

        VertexBufferCreateInfo vbo = {};
        vbo.DebugName = "InstancedQuadMesh";
        vbo.IsShared = false;
        vbo.Layout = &meshLayout;
        vbo.Size = sizeof(vertices);
        vbo.Data = vertices;

        VertexBufferCreateInfo instanceVBO = {};
        instanceVBO.DebugName = "Transforms";
        instanceVBO.IsShared = false;
        instanceVBO.Size = sizeof(glm::vec4) * 4 * size * size;

        s_Resources.QuadBuffer = VertexBuffer::Create(&vbo);
        s_Resources.InstanceBuffer = VertexBuffer::Create(&instanceVBO);

        IndexBufferCreateInfo ibo = {};
        ibo.DebugName = "TriangleIBO";
        ibo.Usage = BufferUsage::StaticDraw;
        ibo.Size = sizeof(indices);
        ibo.Data = indices;

        s_Resources.QuadIndexBuffer = IndexBuffer::Create(&ibo);

        s_Resources.QuadMesh = Ref<RawMesh>::Create(s_Resources.QuadBuffer, s_Resources.QuadIndexBuffer);
        m_CommandBuffer = s_Context->GetSwapchain()->GetSwapchainBuffer();
#endif
    }
}

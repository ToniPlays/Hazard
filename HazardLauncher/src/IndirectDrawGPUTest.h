
#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"
#include <filesystem>

#include <glad/glad.h>

using namespace HazardRenderer;

namespace IndirectDrawGPUTest {

    //OpenGL: Test
    //Vulkan: Working
    //Metal : Test
    //DX12  : Test
    //DX11  : Test

    static void Run(RenderAPI api)
    {
        static bool running = true;

        Window* window = CreateTestWindow("Indirect GPU draw test", api, &running);
        window->Show();
        //---------------

        std::cout << "Selected device: " << window->GetContext()->GetDevice()->GetDeviceName() << std::endl;
        float vertices[] =
        {
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        };
        uint32_t indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        BufferLayout layout = { { "a_Position",	ShaderDataType::Float3 },
                                { "a_Color",	ShaderDataType::Float4 },
                                { "a_MRow1",	ShaderDataType::Float4, PerInstance },
                                { "a_MRow2",	ShaderDataType::Float4, PerInstance },
                                { "a_MRow3",	ShaderDataType::Float4, PerInstance },
        };

        std::vector<InstanceTransform> transforms(4);
        {
            glm::mat4 t = glm::translate(glm::mat4(1.0f), { -1.0f, 1.0f, -3.0f });

            transforms[0].MRow0 = { t[0][0], t[1][0], t[2][0], t[3][0] };
            transforms[0].MRow1 = { t[0][1], t[1][1], t[2][1], t[3][1] };
            transforms[0].MRow2 = { t[0][2], t[1][2], t[2][2], t[3][2] };
        }
        {
            glm::mat4 t = glm::translate(glm::mat4(1.0f), { 1.0f, 1.0f, -3.0f });
            transforms[1].MRow0 = { t[0][0], t[1][0], t[2][0], t[3][0] };
            transforms[1].MRow1 = { t[0][1], t[1][1], t[2][1], t[3][1] };
            transforms[1].MRow2 = { t[0][2], t[1][2], t[2][2], t[3][2] };
        }
        {
            glm::mat4 t = glm::translate(glm::mat4(1.0f), { 1.0f, -1.0f, -3.0f });
            transforms[2].MRow0 = { t[0][0], t[1][0], t[2][0], t[3][0] };
            transforms[2].MRow1 = { t[0][1], t[1][1], t[2][1], t[3][1] };
            transforms[2].MRow2 = { t[0][2], t[1][2], t[2][2], t[3][2] };
        }
        {
            glm::mat4 t = glm::translate(glm::mat4(1.0f), { -1.0f, -1.0f, -3.0f });
            transforms[3].MRow0 = { t[0][0], t[1][0], t[2][0], t[3][0] };
            transforms[3].MRow1 = { t[0][1], t[1][1], t[2][1], t[3][1] };
            transforms[3].MRow2 = { t[0][2], t[1][2], t[2][2], t[3][2] };
        }

        std::vector<ShaderStageCode> code = ShaderCompiler::GetShaderBinariesFromSource("assets/Shaders/instancing.glsl", api);
        std::vector<ShaderStageCode> computeCode = ShaderCompiler::GetShaderBinariesFromSource("assets/Shaders/drawcommandgen.glsl", api);

        std::vector<DrawIndirectIndexedCommand> drawCommands(4);
        drawCommands[0].FirstInstance = 0;
        drawCommands[0].FirstIndex = 0;
        drawCommands[0].IndexCount = sizeof(indices) / sizeof(indices[0]);
        drawCommands[0].InstanceCount = 1;
        drawCommands[0].VertexOffset = 0;

        drawCommands[1].FirstInstance = 1;
        drawCommands[1].FirstIndex = 0;
        drawCommands[1].IndexCount = sizeof(indices) / sizeof(indices[0]);
        drawCommands[1].InstanceCount = 1;
        drawCommands[1].VertexOffset = 0;

        drawCommands[2].FirstInstance = 2;
        drawCommands[2].FirstIndex = 0;
        drawCommands[2].IndexCount = sizeof(indices) / sizeof(indices[0]);
        drawCommands[2].InstanceCount = 1;
        drawCommands[2].VertexOffset = 0;

        drawCommands[3].FirstInstance = 3;
        drawCommands[3].FirstIndex = 0;
        drawCommands[3].IndexCount = sizeof(indices) / sizeof(indices[0]);
        drawCommands[3].InstanceCount = 1;
        drawCommands[3].VertexOffset = 0;

        VertexBufferCreateInfo vbo = {};
        vbo.Name = "TriangleVBO";
        vbo.Layout = &layout;
        vbo.Size = sizeof(vertices);
        vbo.Data = &vertices;

        VertexBufferCreateInfo instanceBufferSpec = {};
        instanceBufferSpec.Name = "InstanceVBO";
        instanceBufferSpec.Size = sizeof(InstanceTransform) * transforms.size();
        instanceBufferSpec.Data = transforms.data();

        IndexBufferCreateInfo ibo = {};
        ibo.Name = "TriangleIBO";
        ibo.Size = sizeof(indices);
        ibo.Data = indices;

        ArgumentBufferCreateInfo argumentBufferSpec = {};
        argumentBufferSpec.Name = "ArgumentBuffer";
        argumentBufferSpec.Size = sizeof(DrawIndirectIndexedCommand) * drawCommands.size();
        argumentBufferSpec.Data = drawCommands.data();

        ArgumentBufferCreateInfo drawCommandBufferSpec = {};
        drawCommandBufferSpec.Name = "DrawCommandBuffer";
        drawCommandBufferSpec.Size = 64 * sizeof(DrawIndirectIndexedCommand);

        PipelineSpecification spec = {};
        spec.DebugName = "Pipeline";
        spec.Usage = PipelineUsage::GraphicsBit;
        spec.DrawType = DrawType::Fill;
        spec.CullMode = CullMode::None;
        spec.pTargetRenderPass = window->GetSwapchain()->GetRenderPass().Raw();
        spec.DepthTest = false;
        spec.pBufferLayout = &layout;
        spec.ShaderCodeCount = code.size();
        spec.pShaderCode = code.data();

        PipelineSpecification computeSpec = {};
        computeSpec.DebugName = "ComputePipeline";
        computeSpec.Usage = PipelineUsage::ComputeBit;
        computeSpec.ShaderCodeCount = computeCode.size();
        computeSpec.pShaderCode = computeCode.data();

        UniformBufferCreateInfo uboInfo = {};
        uboInfo.Name = "Camera";
        uboInfo.Set = 0;
        uboInfo.Binding = 0;
        uboInfo.Size = sizeof(glm::mat4);

        {
            Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(&vbo);
            Ref<VertexBuffer> instancedBuffer = VertexBuffer::Create(&instanceBufferSpec);
            Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(&ibo);
            Ref<ArgumentBuffer> argumentBuffer = ArgumentBuffer::Create(&argumentBufferSpec);
            Ref<ArgumentBuffer> drawCommandBuffer = ArgumentBuffer::Create(&drawCommandBufferSpec);
            Ref<Pipeline> pipeline = Pipeline::Create(&spec);
            Ref<Pipeline> compute = Pipeline::Create(&computeSpec);
            Ref<UniformBuffer> camera = UniformBuffer::Create(&uboInfo);

            auto shader = compute->GetShader();
            shader->Set("InDrawCommands", 0, argumentBuffer.As<BufferBase>());
            shader->Set("OutDrawCommands", 0, drawCommandBuffer.As<BufferBase>());

            while (running)
            {
                auto swapchain = window->GetSwapchain();
                auto commandBuffer = swapchain->GetSwapchainBuffer();
                auto renderPass = swapchain->GetRenderPass();

                float aspectRatio = (float)window->GetWidth() / (float)window->GetHeight();
                glm::mat4 projection = glm::perspective(glm::radians(60.0f), aspectRatio, 0.03f, 100.0f);

                {
                    BufferCopyRegion region = {};
                    region.Data = &projection;
                    region.Size = sizeof(glm::mat4);
                    camera->SetData(region);
                }

                Input::Update();
                window->BeginFrame();

                DispatchComputeInfo computeInfo = {};
                computeInfo.GroupSize = { 4, 1, 1 };
                computeInfo.WaitForCompletion = true;

                {
                    uint64_t data = 0;

                    BufferCopyRegion region = {};
                    region.Data = &data;
                    region.Size = sizeof(uint64_t);
                    region.Offset = 0;

                    drawCommandBuffer->SetData(region);
                }

                commandBuffer->SetPipeline(compute);
                commandBuffer->DispatchCompute(computeInfo);

                commandBuffer->BeginRenderPass(renderPass);
                commandBuffer->SetVertexBuffer(vertexBuffer);
                commandBuffer->SetVertexBuffer(instancedBuffer, 1);
                commandBuffer->SetPipeline(pipeline);
                commandBuffer->DrawIndirect(drawCommandBuffer, 4, sizeof(DrawIndirectIndexedCommand), 1, indexBuffer);
                commandBuffer->EndRenderPass();

                Renderer::WaitAndRender();
                window->Present();
            }
        }
        window->Close();
    }
}



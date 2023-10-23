
#include "HazardRenderer.h"
#include "Application/Test.h"

class TexturedQuadTest : public Test
{
public:
	void Reset() override;
	void Init() override;
	void Run() override;
	void Terminate() override;

	const char* GetName() override { return "Textured quad test"; }

private:
	HazardRenderer::Window* m_Window;
	Ref<HazardRenderer::GPUBuffer> m_VertexBuffer;
	Ref<HazardRenderer::GPUBuffer> m_IndexBuffer;
	Ref<HazardRenderer::DescriptorSet> m_DescriptorSet;
	Ref<HazardRenderer::Image2D> m_Image;
	Ref<HazardRenderer::Sampler> m_Sampler;
	Ref<HazardRenderer::Pipeline> m_Pipeline;
};


/*
	static void Run(RenderAPI api)
	{
		static bool running = true;

		Window* window = CreateTestWindow("Textured quad", api, &running);
		window->Show();

		//---------------
		

		while (running)
		{
            auto swapchain = window->GetSwapchain();
            auto commandBuffer = swapchain->GetSwapchainBuffer();
            auto renderPass = swapchain->GetRenderPass();

			Input::Update();
			window->BeginFrame();
            
			commandBuffer->BeginRenderPass(renderPass);
			commandBuffer->SetPipeline(pipeline);
			commandBuffer->SetDescriptorSet(descriptorSet, 0);
			commandBuffer->SetVertexBuffer(vertexBuffer);
			commandBuffer->Draw(indexBuffer->GetSize() / sizeof(uint32_t), indexBuffer);
			commandBuffer->EndRenderPass();
            
			Renderer::WaitAndRender();
			window->Present();
		}
		window->Close();
	}
*/

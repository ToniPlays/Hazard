
#include "HazardRenderer.h"
#include "Application/Test.h"

class ComputeShaderTest : public Test
{
public:
    void Reset() override;
    void Init() override;
    void Run() override;
    void Terminate() override;

    const char* GetName() override { return "Compute shader test"; }

private:
    HazardRenderer::Window* m_Window;
    Ref<HazardRenderer::GPUBuffer> m_VertexBuffer;
    Ref<HazardRenderer::Pipeline> m_Pipeline;
    Ref<HazardRenderer::Pipeline> m_ComputePipeline;
    Ref<HazardRenderer::DescriptorSet> m_DescriptorSet;
    Ref<HazardRenderer::DescriptorSet> m_ComputeDescriptorSet;
    Ref<HazardRenderer::Image2D> m_OutputImage;
    Ref<HazardRenderer::Sampler> m_Sampler;
};

/*

		{
			Ref<RenderCommandBuffer> computeBuffer = RenderCommandBuffer::Create("Compute", DeviceQueue::ComputeBit, 1);

			//Compute step
			{
				computeDescriptorSet->Write(0, outputImage, sampler);

				computeBuffer->Begin();
				computeBuffer->SetPipeline(compute);
				computeBuffer->SetDescriptorSet(computeDescriptorSet, 0);
				computeBuffer->DispatchCompute({ 512, 512, 1 });
				computeBuffer->End();
				computeBuffer->Submit();
			}

			descriptorSet->Write(0, 0, outputImage, sampler, true);
		}
	}
}*/


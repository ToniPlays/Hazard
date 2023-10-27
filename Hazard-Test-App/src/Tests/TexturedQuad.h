
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

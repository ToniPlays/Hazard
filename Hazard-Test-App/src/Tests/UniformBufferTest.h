
#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"
#include "Application/Test.h"

#include "vendor/stb_image.h"
#include <glm/glm/ext/matrix_clip_space.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

using namespace HazardRenderer;


class UniformBufferTest : public Test
{
public:
	void Reset() override;
	void Init() override;
	void Run() override;
	void Terminate() override;

	const char* GetName() override { return "Uniform buffer test"; }

private:
	HazardRenderer::Window* m_Window;
	Ref<HazardRenderer::GPUBuffer> m_VertexBuffer;
	Ref<HazardRenderer::GPUBuffer> m_IndexBuffer;
	Ref<HazardRenderer::GPUBuffer> m_UniformBuffer;
	Ref<HazardRenderer::DescriptorSet> m_DescriptorSet;
	Ref<HazardRenderer::Image2D> m_Image;
	Ref<HazardRenderer::Sampler> m_Sampler;
	Ref<HazardRenderer::Pipeline> m_Pipeline;
};


#include "HazardRenderer.h"
#include "Test.h"

class TriangleTest : public Test
{
public:
	void Reset() override;
	void Init() override;
	void Run() override;
	void Terminate() override;

	const char* GetName() { return "Triangle test"; }

private:
	HazardRenderer::Window* m_Window;
	Ref<HazardRenderer::GPUBuffer> m_VertexBuffer;
	Ref<HazardRenderer::Pipeline> m_Pipeline;
};

#include "TriangleTest.h"
#include "TexturedQuad.h"
#include "UniformBufferTest.h"
#include "AccelerationStructureTest.h"
#include "RayTracingSphere.h"

#include "JobSystemTest.h"

int main(int argc, char** argv)
{
    uint32_t api = 0;
#if !defined(HZR_PLATFORM_IOS)
	std::cout << "Select api" << std::endl;
	std::cout << " 0 - Auto" << std::endl;
	std::cout << " 1 - OpenGL" << std::endl;
	std::cout << " 2 - Vulkan" << std::endl;
	std::cout << " 3 - Metal" << std::endl;
	
	std::cin >> api;
	
#endif

	JobSystemTest::Run();
	return 0;

    RenderAPI renderAPI = (RenderAPI)api;
	TriangleTest::Run(renderAPI);
	TexturedQuad::Run(renderAPI);
	UniformBufferTest::Run(renderAPI);

	if (renderAPI == RenderAPI::Vulkan)
	{
		AccelerationStructureTest::Run(renderAPI);
		RayTracingSphere::Run(renderAPI);
	}
	RayTracingSphere::Run(renderAPI);
    return 0;
}

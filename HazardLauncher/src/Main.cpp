
#include "TriangleTest.h"
#include "TexturedQuad.h"
#include "UniformBufferTest.h"
#include "ComputeShaderTest.h"
#include "InstancingTest.h"
#include "AccelerationStructureTest.h"
#include "RayTracingSphere.h"
#include "IndirectDrawCPUTest.h"
#include "IndirectDrawGPUTest.h"

#include "JobSystemTest.h"

#include <filesystem>

int main(int argc, char** argv)
{
    
    uint32_t api = 0;
    
#if !defined(HZR_PLATFORM_IOS)
	std::cout << "---- Select api ----" << std::endl;
	std::cout << " 0 - Auto" << std::endl;
	std::cout << " 1 - OpenGL" << std::endl;
	std::cout << " 2 - Vulkan" << std::endl;
	std::cout << " 3 - Metal" << std::endl;
	
	std::cin >> api;
#endif
    RenderAPI renderAPI = (RenderAPI)api;

	std::cout << "---- Select test ----\n";
	std::cout << "1. JobSystemTest\n";
	std::cout << "2. TriangleTest\n";
	std::cout << "3. TexturedQuad\n";
	std::cout << "4. UniformBufferTest\n";
	std::cout << "5. ComputeShaderTest\n";
	std::cout << "6. InstancingTest\n";
	std::cout << "7. IndirectDrawCPUTest\n";
	std::cout << "8. IndirectDrawGPUTest\n";
	std::cout << "9. AccelerationStructureTest\n";
	std::cout << "10. RayTracingSphere\n";

	uint32_t test = 0;
	std::cin >> test;

	switch (test)
	{
	case 1:
		JobSystemTest::Run();
		break;
	case 2:
		TriangleTest::Run(renderAPI);
		break;
	case 3:
		TexturedQuad::Run(renderAPI);
		break;
	case 4:
		UniformBufferTest::Run(renderAPI);
		break;
	case 5:
	    ComputeShaderTest::Run(renderAPI);
		break;
	case 6:
		InstancingTest::Run(renderAPI);
		break;
	case 7:
	    IndirectDrawCPUTest::Run(renderAPI);
		break;
	case 8:
		IndirectDrawGPUTest::Run(renderAPI);
		break;
	case 9:
		AccelerationStructureTest::Run(renderAPI);
		break;
	case 10:
		RayTracingSphere::Run(renderAPI);
		break;
	}
    return 0;
}

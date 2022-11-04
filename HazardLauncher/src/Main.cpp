
#include "Tests/TriangleTest.h"
#include "Tests/TexturedQuad.h"
#include "Tests/UniformBufferTest.h"
#include "Tests/AccelerationStructureTest.h"

void main(int argc, char* argv)
{
	std::cout << "Select api" << std::endl;
	std::cout << " 0 - Auto" << std::endl;
	std::cout << " 1 - OpenGL" << std::endl;
	std::cout << " 2 - Vulkan" << std::endl;
	std::cout << " 3 - Metal" << std::endl;

	uint32_t api;
	std::cin >> api;
	RenderAPI renderAPI = (RenderAPI)api;

	AccelerationStructureTest::Run(renderAPI);
}
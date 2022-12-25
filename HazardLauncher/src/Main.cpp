
#include "tests/TriangleTest.h"
#include "tests/TexturedQuad.h"
#include "tests/UniformBufferTest.h"
#include "tests/AccelerationStructureTest.h"
#include "tests/RayTracingSphere.h"

int main(int argc, char** argv)
{
	std::cout << "Select api" << std::endl;
	std::cout << " 0 - Auto" << std::endl;
	std::cout << " 1 - OpenGL" << std::endl;
	std::cout << " 2 - Vulkan" << std::endl;
	std::cout << " 3 - Metal" << std::endl;

	uint32_t api;
	std::cin >> api;
	RenderAPI renderAPI = (RenderAPI)api;

	TexturedQuad::Run(renderAPI);
    return 0;
}

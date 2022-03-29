#include "tests/SimpleCanvas.cpp"
#include "tests/Rendering/TriangleTest.h"
#include "tests/Rendering/TexturedQuad.h"
#include "tests/Rendering/InstancingTest.h"


int main()
{
	InstancingTest::Run(RenderAPI::OpenGL);
	std::cout << "Test endeds" << std::endl;
	return 0;
}

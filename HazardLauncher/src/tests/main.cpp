#include "tests/SimpleCanvas.cpp"
#include "tests/Rendering/TriangleTest.h"
#include "tests/Rendering/TexturedQuad.h"
#include "tests/Rendering/InstancingTest.h"
#include "tests/Rendering/RenderCommandTest.h"


int main()
{
	RenderCommandTest::Run(RenderAPI::OpenGL);
	std::cout << "Test ended yes" << std::endl;
	return 0;
}

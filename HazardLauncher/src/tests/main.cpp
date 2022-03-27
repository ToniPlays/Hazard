#include "tests/SimpleCanvas.cpp"
#include "tests/Rendering/TriangleTest.h"
#include "tests/Rendering/TexturedQuad.h"


int main()
{
	TexturedQuad::Run(RenderAPI::Vulkan);
	return 0;
}

#include "tests/SimpleCanvas.cpp"
#include "tests/Rendering/TriangleTest.h"
#include "tests/Rendering/TexturedQuad.h"


int main()
{
	TriangleTest::Run(RenderAPI::Metal);
	return 0;
}

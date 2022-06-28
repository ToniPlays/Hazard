#if 1
#include "tests/SimpleCanvas.cpp"
#include "tests/Rendering/TriangleTest.h"
#include "tests/Rendering/TexturedQuad.h"
#include "tests/Rendering/InstancingTest.h"
#include "tests/Rendering/RenderCommandTest.h"

#include "tests/Scripting/CoreTest.h"


int main()
{
	TexturedQuad::Run(RenderAPI::Metal);
	std::cout << "Test ended " << std::endl;
	return 0;
}

#endif

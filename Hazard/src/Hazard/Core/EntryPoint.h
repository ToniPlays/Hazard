#pragma once

#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include "Hazard/Core/HazardLoop.h"

#include <stdio.h>

uint32_t allocs = 0;

void* operator new(size_t size)
{
	allocs++;
	return malloc(size);
}

extern Hazard::Application* Hazard::CreateApplication();

#ifdef HZR_PLATFORM_WINDOWS

	#ifdef HZR_DEBUG

		#pragma comment( linker, "/subsystem:console" )
		int main()
		{
			using namespace Hazard;
			Application* app = CreateApplication();
			Core::HazardLoop loop(app);
			loop.Start();
			while (!loop.ShouldClose()) {
				loop.Run();
			}

			return 0;
		}

	#else
		#pragma comment( linker, "/subsystem:windows" )
		int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
		{
			using namespace Hazard;
			Application* app = CreateApplication();
			Core::HazardLoop loop(app);
			loop.Start();
			while (!loop.ShouldClose()) {
				loop.Run();
				std::cout << allocs << std::endl;
			}
			return 0;
		}
	#endif // HZR_DEBUG

#else
	#error Unsupported platform for Hazard
#endif // HZR_PLATFORM_WINDOWS
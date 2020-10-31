#pragma once
#include "HazardLoop.h"
#include "Hazard/Core/Application.h"
#include <io.h>
#include <fcntl.h>
#include <windows.h>


/*struct MemoryUsage {
	size_t usedMemory = 0;
};

MemoryUsage memory;

void* operator new(size_t size) {
	memory.usedMemory += size;
	return malloc(size);
}
void operator delete(void* loc, size_t size) {
	memory.usedMemory -= size;
	free(loc);
}

*/
#ifdef HZR_PLATFORM_WINDOWS

	extern Hazard::Application* Hazard::CreateApplication();
#ifdef HZR_DEBUG
#pragma comment( linker, "/subsystem:console" )
	int main()
#else
#pragma comment( linker, "/subsystem:windows" )
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow)
#endif // HZR_DEBUG
	{
		Hazard::Application* app = Hazard::CreateApplication();
		Hazard::HazardLoop* hazardLoop = new Hazard::HazardLoop(app);
		hazardLoop->Run();
		delete hazardLoop;
	}
#else
#error Unsupported platform
#endif // HZR_PLATFORM_WINDOWS
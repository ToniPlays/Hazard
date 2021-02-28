#pragma once

#include <io.h>
#include <fcntl.h>
#include <windows.h>
#include "Hazard/Core/HazardLoop.h"

#include "imgui.h"
#include "GLFW/imgui_impl_glfw.h"
#include "GLFW/imgui_impl_opengl3.h"
#include <stdio.h>

#include "GLFW/glfw3.h"
#include "glad/glad.h"

uint64_t memorySize = 0;
/*
void* operator new(size_t size) {
	memorySize += size;
	void* p = malloc(size);
	return p;
}
void operator delete(void* p) {
	memorySize -= sizeof(p);
	free(p);
}
*/

#ifdef HZR_PLATFORM_WINDOWS

extern Hazard::Application* Hazard::CreateApplication();
#if defined(HZR_DEBUG) || defined(HZR_GAME_ONLY)
	#pragma comment( linker, "/subsystem:console" )
	int main()
#else
	#pragma comment( linker, "/subsystem:windows" )
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif // HZR_DEBUG
{
	Hazard::Application* app = Hazard::CreateApplication();
	Hazard::Core::HazardLoop loop(app);
	loop.Start();
}
#else
	#error Unsupported platform for Hazard
#endif // HZR_PLATFORM_WINDOWS
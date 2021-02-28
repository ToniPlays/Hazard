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

#ifdef HZR_PLATFORM_WINDOWS

extern Hazard::Application* Hazard::CreateApplication();
#if defined(HZR_DEBUG) || defined(HZR_GAME_ONLY)
	//Appliction starts as console-app
	#pragma comment( linker, "/subsystem:console" )
	int main()
#else
	//Gets rid of the console window
	#pragma comment( linker, "/subsystem:windows" )
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif // HZR_DEBUG
{
	//Create Hazard application from external definition
	Hazard::Application* app = Hazard::CreateApplication();
	//Create HazardLoop and run it
	Hazard::Core::HazardLoop loop(app);
	loop.Start();
}
#else
	#error Unsupported platform for Hazard
#endif // HZR_PLATFORM_WINDOWS
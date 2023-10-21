#pragma once

#ifdef HZR_PLATFORM_WINDOWS

#include <io.h>
#include <fcntl.h>
#include <windows.h>

#include "Profiling/MemoryDiagnostic.h"

#include "Hazard/Core/HazardLoop.h"
#include "Application.h"
#include "Hazard/Core/CommandLineArgs.h"

#include <stdio.h>

extern Hazard::Application* Hazard::CreateApplication();

#ifdef HZR_DEBUG

#pragma comment( linker, "/subsystem:console" )
int main(int argc, char* argv[])
{
	using namespace Hazard;
	{
	#ifdef HZR_MEM_DIAG
		Memory::Allocator::Init();
	#endif
		CommandLineArgs::Init(argc, argv);

		Application* app = CreateApplication();

		HazardLoop loop(app);
		loop.Start();

		while (!loop.ShouldClose())
			loop.Run();
		loop.Close();
	}
	return 0;
}

#else

#pragma comment( linker, "/subsystem:windows" )
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	using namespace Hazard;

	CommandLineArgs::Init(std::string(lpCmdLine));

	Application* app = CreateApplication();

	HazardLoop loop(app);
	loop.Start();

	while (!loop.ShouldClose())
		loop.Run();
	return 0;
}
#endif
#endif
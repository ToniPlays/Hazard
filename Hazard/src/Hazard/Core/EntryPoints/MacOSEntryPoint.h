#pragma once

#ifdef HZR_PLATFORM_MACOS

#include "Profiling/MemoryDiagnostic.h"

#include "Hazard/Core/HazardLoop.h"
#include "Hazard/Core/Application.h"
#include "Hazard/Core/CommandLineArgs.h"

extern Hazard::Application* Hazard::CreateApplication();

int main(int argc, char* argv[])
{
	using namespace Hazard;
	{
		CommandLineArgs::Init(argc, argv);
		Application* app = CreateApplication();

		HazardLoop loop(app);
		loop.Start();

		while (!loop.ShouldClose())
        {
			loop.Run();
		}
	}
	return 0;
}
#endif // HZR_PLATFORM_MACOS

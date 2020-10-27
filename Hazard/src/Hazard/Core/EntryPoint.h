#pragma once
#include "HazardLoop.h"
#include "Hazard/Core/Application.h"
#include <io.h>
#include <fcntl.h>
#include <windows.h>

#ifdef HZR_PLATFORM_WINDOWS

	extern Hazard::Application* Hazard::CreateApplication();
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
		LPSTR lpCmdLine, int nCmdShow) {

#ifdef HZR_DEBUG | HZR_DIST
        AllocConsole();

        HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
        int hCrt = _open_osfhandle((long)handle_out, _O_TEXT);
        FILE* hf_out = _fdopen(hCrt, "w");
        setvbuf(hf_out, NULL, _IONBF, 1);
        *stdout = *hf_out;

        HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
        hCrt = _open_osfhandle((long)handle_in, _O_TEXT);
        FILE* hf_in = _fdopen(hCrt, "r");
        setvbuf(hf_in, NULL, _IONBF, 128);
        *stdin = *hf_in;
#endif
		Hazard::Application* app = Hazard::CreateApplication();
		Hazard::HazardLoop* hazardLoop = new Hazard::HazardLoop(app);
		hazardLoop->Run();
		delete hazardLoop;
	}
#else
#error Unsupported platform
#endif // HZR_PLATFORM_WINDOWS
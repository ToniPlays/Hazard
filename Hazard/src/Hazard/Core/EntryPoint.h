#pragma once

#ifndef HZR_CUSTOM_ENTRY

    #ifdef HZR_PLATFORM_WINDOWS
    #include <io.h>
    #include <fcntl.h>
    #include <windows.h>
    #endif
    #include "Hazard/Core/HazardLoop.h"
    #include "Hazard/Core/CommandLineArgs.h"

    #include <stdio.h>
    //#include <crtdbg.h>

    extern Hazard::Application* Hazard::CreateApplication();

    #ifdef HZR_PLATFORM_WINDOWS

        #ifdef HZR_DEBUG

            #pragma comment( linker, "/subsystem:console" )
            int main(int argc, char* argv[])
            {
                //_CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF);
                using namespace Hazard;
                {
                    CommandLineArgs::Init(argc, argv);
                    Application* app = CreateApplication();

                    Core::HazardLoop loop(app);
                    loop.Start();

                    while (!loop.ShouldClose()) {
                        loop.Run();
                    }
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
                }
                return 0;
            }
        #endif // HZR_DEBUG
    #else
        int main(int argc, char* argv[])
        {
            using namespace Hazard;

            CommandLineArgs::Init(argc, argv);
            Application* app = CreateApplication();

            Core::HazardLoop loop(app);
            loop.Start();
            while (!loop.ShouldClose()) {
                loop.Run();
            }

            return 0;
        }
#endif // HZR_PLATFORM_WINDOWS
#endif

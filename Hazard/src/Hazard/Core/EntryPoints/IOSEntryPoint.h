#ifdef HZR_PLATFORM_IOS
#import "Hazard/Core/Application.h"

extern Hazard::Application* Hazard::CreateApplication();

Hazard::HazardLoop* loop = nullptr;

int main(int argc, char* arcv[]);

static void IOSInitApplication()
{
    using namespace Hazard;
    
    Application* app = CreateApplication();
    loop = hnew HazardLoop(app);
    loop->Start();
}

static void IOSRunApplication()
{
    using namespace Hazard;
    while (!loop->ShouldClose())
        loop->Run();
}

#endif

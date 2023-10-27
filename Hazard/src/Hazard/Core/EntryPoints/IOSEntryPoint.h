#ifdef HZR_PLATFORM_IOS

#import <UIKit/UIKit.h>
#import "AppDelegate.h"

#import "../CommandLineArgs.h"
#import "Hazard/Core/Application.h"

extern Hazard::Application* Hazard::CreateApplication();

Hazard::HazardLoop loop;

int main(int argc, char * argv[])
{
    Hazard::CommandLineArgs::Init(argc, argv);
    
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}
static void IOSInitApplication()
{
    using namespace Hazard;
    Application* app = CreateApplication();
    loop.SetApplication(app);
    loop.Start();
}
static void IOSRunApplication()
{
    using namespace Hazard;
    {
        while (!loop.ShouldClose())
        {
            loop.Run();
        }
    }
}

#endif

#ifdef HZR_PLATFORM_IOS

#import <UIKit/UIKit.h>
#import "AppDelegate.h"

#import "../CommandLineArgs.h"
#import "Hazard/Core/Application.h"

extern Hazard::Application* Hazard::CreateApplication();

int main(int argc, char * argv[]) 
{
    
    Hazard::CommandLineArgs::Init(argc, argv);
    
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}
static void IOSRunApplication()
{
    using namespace Hazard;
    {
        Application* app = CreateApplication();

        HazardLoop loop(app);
        loop.Start();

        while (!loop.ShouldClose())
        {
            loop.Run();
        }
    }
}

#endif

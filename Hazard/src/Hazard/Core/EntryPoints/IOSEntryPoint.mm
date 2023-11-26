#ifdef HZR_PLATFORM_IOS
#include "IOSEntryPoint.h"

#include "../CommandLineArgs.h"
#import <UIKit/UIKit.h>
#import "AppDelegate.h"

int main(int argc, char * argv[])
{
    Hazard::CommandLineArgs::Init(argc, argv);
    
    @autoreleasepool {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}

#endif

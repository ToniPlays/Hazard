
#include "IOSViewController.h"
#ifdef HZR_PLATFORM_IOS

#include "IOSEntryPoint.h"

@implementation IOSViewController
{
}

- (void)OnViewLoaded
{
    printf("%s", "Override ViewController\n");
    [self performSelectorInBackground:@selector(RunApplication) withObject: nil];
    
}
- (void)RunApplication
{
    IOSInitApplication();
    IOSRunApplication();
}

@end

#endif

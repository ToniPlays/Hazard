

#ifdef HZR_PLATFORM_IOS

#import "DefaultViewController.h"
#import "MetalKit/MetalKit.h"
#import "IOSEntryPoint.h"

@implementation DefaultViewController
{
    MTKView* m_MTKView;
}

- (void)viewDidAppear:(BOOL)animated {
    m_MTKView = (MTKView*)self.view;
    
    IOSRunApplication();
}

@end

#endif

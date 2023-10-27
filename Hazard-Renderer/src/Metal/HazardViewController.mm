
#ifdef HZR_PLATFORM_IOS

#include <iostream>
#import "HazardViewController.h"
#import "GestureDelegate.h"

@implementation HazardViewController
{
    MTKView* m_MTKView;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    std::cout << "View appeared" << std::endl;
    m_MTKView = (MTKView*)self.view;
    
    [self SetupRecognizers];
    [self OnViewLoaded];
}

- (MTKView *)GetMTKView
{
    return m_MTKView;
}

- (void)OnViewLoaded
{
    printf("%s", "Must override ViewController");
}
- (void)SetupRecognizers
{
    GestureDelegate* delegate = [[GestureDelegate alloc] initWithTarget: self action: @selector(handleGesture:)];
    
    [[self view] addGestureRecognizer: delegate];
}
- (void)handleGesture:(GestureDelegate*)delegate
{
    
}
@end

#endif

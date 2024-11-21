
#ifdef HZR_PLATFORM_IOS

#include <iostream>
#import "HazardViewController.h"
#import "GestureDelegate.h"
#include "Core/Window.h"
#include "Input.h"
#include "IOSUtils.h"

@implementation HazardViewController
{
    MTKView* m_MTKView;
    HazardRenderer::IOSWindow* m_Window;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
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
    HZR_ASSERT(false, "Must override ViewController");
}

- (void)SetWindow: (HazardRenderer::IOSWindow*)window
{
    m_Window = window;
}

- (void)SetupRecognizers
{
    GestureDelegate* delegate = [[GestureDelegate alloc] initWithTarget: self action: @selector(handleGesture:)];
    
    [[self view] addGestureRecognizer: delegate];
    
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(orientationChanged:) name:UIDeviceOrientationDidChangeNotification object:[UIDevice currentDevice]];
    
}
- (void)handleGesture:(GestureDelegate*)delegate
{
    switch(delegate.state)
    {
        case UIGestureRecognizerStateBegan:
        {
            MouseButtonPressedEvent e(0);
            Input::OnEvent(e);
            break;
        }
        case UIGestureRecognizerStateEnded:
        {
            MouseButtonReleasedEvent e(0);
            Input::OnEvent(e);
            break;
        }
        case UIGestureRecognizerStateChanged:
        {
            CGSize screenSize = [[UIScreen mainScreen] nativeBounds].size;
            CGSize size = self.view.frame.size;
            CGPoint coord = [delegate locationInView: self.view];
            
            MouseMovedEvent e((coord.x / size.width) * screenSize.height, (coord.y / size.height) * screenSize.width);
            Input::OnEvent(e);
            break;
        }
        default: return;
    }
}
- (void)orientationChanged: (NSNotification*) note
{
    using namespace HazardRenderer;
    UIDevice* device = note.object;
    Orientation orientation = IOSUtils::GetWindowOrientation(0);
    if((uint8_t)orientation == 0) return;
    
    HazardRenderer::IOSUtils::OnDeviceOrientationChanged(orientation);
}

@end

#endif

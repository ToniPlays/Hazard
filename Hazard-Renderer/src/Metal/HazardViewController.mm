
#ifdef HZR_PLATFORM_IOS

#include <iostream>
#import "HazardViewController.h"
#import "GestureDelegate.h"
#include "Input.h"

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
@end

#endif

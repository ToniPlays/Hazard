
#ifdef HZR_PLATFORM_IOS

#include <iostream>
#import "HazardViewController.h"
#import "Metal/MetalSwapchain.h"

@implementation HazardViewController
{
    MTKView* m_MTKView;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    std::cout << "View appeared" << std::endl;
    m_MTKView = (MTKView*)self.view;
    m_MTKView.multipleTouchEnabled = YES;
    m_MTKView.autoResizeDrawable = NO;
    
    [self OnViewLoaded];
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    UITouch* touch = [[touches allObjects] anyObject];
    
    CGPoint point = [touch locationInView: m_MTKView];
    
    std::cout << "Touches started: " << touches.count << std::endl;
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    std::cout << "Touches ended: " << touches.count << std::endl;
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    std::cout << "Touches moved: " << touches.count << std::endl;
}

- (MTKView *)GetMTKView
{
    return m_MTKView;
}

- (void)OnViewLoaded
{
    printf("%s", "Must override ViewController");
}

@end

#endif

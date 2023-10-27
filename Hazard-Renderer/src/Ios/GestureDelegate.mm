
#import "GestureDelegate.h"

#ifdef HZR_PLATFORM_IOS

#import <Foundation/Foundation.h>
#include "Input.h"

@implementation GestureDelegate

-(id)initWithTarget:(id)target action:(SEL)action
{
    if((self = [super initWithTarget:target action:action]))
    {
        
    }
    return self;
}
-(void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    MouseButtonPressedEvent e(0);
    Input::OnEvent(e);
}
-(void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    UITouch* touch = [[touches allObjects] firstObject];
    CGPoint coord = [self locationInView: [self.view superview]];
    MouseMovedEvent e(coord.x, coord.y);
    Input::OnEvent(e);
}
-(void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    MouseButtonReleasedEvent e(0);
    Input::OnEvent(e);
}

@end

#endif

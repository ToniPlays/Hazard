
#import "GestureDelegate.h"

#ifdef HZR_PLATFORM_IOS

#import <Foundation/Foundation.h>

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
    self.state = UIGestureRecognizerStateBegan;
}
-(void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    self.state = UIGestureRecognizerStateChanged;
}
-(void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{    
    self.state = UIGestureRecognizerStateEnded;
}

@end

#endif

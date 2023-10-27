
#import "GestureDelegate.h"
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
    
}
-(void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{

}
-(void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{

}

@end

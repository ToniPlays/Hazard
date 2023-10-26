
#ifdef HZR_PLATFORM_IOS

#import <UIKit/UIKit.h>
#import "MetalKit/MetalKit.h"

// Our iOS view controller
@interface HazardViewController : UIViewController
- (MTKView*) GetMTKView;
- (void) OnViewLoaded;
@end

#endif

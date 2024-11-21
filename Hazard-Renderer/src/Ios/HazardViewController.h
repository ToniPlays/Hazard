
#ifdef HZR_PLATFORM_IOS

#import <UIKit/UIKit.h>
#import "MetalKit/MetalKit.h"
#include "IOS/IOSWindow.h"

@interface HazardViewController : UIViewController
- (MTKView*) GetMTKView;
- (void) OnViewLoaded;
- (void) SetWindow: (HazardRenderer::IOSWindow*)window;
@end

#endif

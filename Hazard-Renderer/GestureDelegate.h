#pragma once

#include "Core/Core.h"

#ifdef HZR_PLATFORM_IOS
#import <UIKit/UIGestureRecognizerSubclass.h>

@interface GestureDelegate : UIGestureRecognizer

@end

#endif

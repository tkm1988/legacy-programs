//
//  iGANeXTModelController.h
//  iGA NeXT
//
//  Created by 片ノ坂 卓磨 on 13/01/30.
//  Copyright (c) 2013年 片ノ坂 卓磨. All rights reserved.
//

#import <UIKit/UIKit.h>

@class iGANeXTDataViewController;

@interface iGANeXTModelController : NSObject <UIPageViewControllerDataSource>

- (iGANeXTDataViewController *)viewControllerAtIndex:(NSUInteger)index storyboard:(UIStoryboard *)storyboard;
- (NSUInteger)indexOfViewController:(iGANeXTDataViewController *)viewController;

@end

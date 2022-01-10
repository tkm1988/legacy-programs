//
//  SimilarityMapController.h
//  iGANext
//
//  Created by 片ノ坂 卓磨 on 13/01/30.
//  Copyright (c) 2013年 片ノ坂 卓磨. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface SimilarityMapController : UIViewController<UINavigationControllerDelegate>
{
  NSMutableDictionary *tshirtColorDict;
  NSMutableDictionary *tshirtCollarDict;
  NSMutableDictionary *tshirtPatternDict;
}

@end

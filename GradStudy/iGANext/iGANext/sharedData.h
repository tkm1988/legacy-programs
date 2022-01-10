//
//  sharedData.h
//  iGANext
//
//  Created by 片ノ坂 卓磨 on 13/01/30.
//  Copyright (c) 2013年 片ノ坂 卓磨. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface sharedData : NSObject{
  NSMutableDictionary *patternToCoordDict;
  NSMutableArray *individualsArr;
  NSMutableDictionary *nextIndividualsDict;
  NSNumber *iterationNum;
}
+ (sharedData *) singleton;
@property (nonatomic, retain) NSMutableDictionary *patternToCoordDict;
@property (nonatomic, retain) NSMutableArray *individualsArr;
@property (nonatomic, retain) NSMutableDictionary *nextIndividualsDict;
@property (nonatomic, retain) NSNumber *iterationNum;
@end

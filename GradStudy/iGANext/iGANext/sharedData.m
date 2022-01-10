//
//  sharedData.m
//  iGANext
//
//  Created by 片ノ坂 卓磨 on 13/01/30.
//  Copyright (c) 2013年 片ノ坂 卓磨. All rights reserved.
//

#import "sharedData.h"

@implementation sharedData
//synthesizeは省略

//シングルトンパターンによる実装
+ (sharedData *) singleton
{
  static sharedData *singleton = nil;
  static dispatch_once_t onceToken;
  
  //一度しか呼ばれないことを明示的に宣言
  dispatch_once(&onceToken, ^{
    singleton = [[sharedData alloc] init];
  });

  return singleton;
}

- (id)init
{
  if (self = [super init]) {
    self.patternToCoordDict = [[NSMutableDictionary alloc] init];
    self.individualsArr = [[NSMutableArray alloc] init];
    self.nextIndividualsDict = [[NSMutableDictionary alloc] init];
    self.iterationNum = [[NSNumber alloc] initWithInteger:0];
  }
  
  return self;
}

@end

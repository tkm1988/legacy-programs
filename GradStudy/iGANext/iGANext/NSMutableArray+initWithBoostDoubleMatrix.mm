//
//  NSMutableArray+NSMutableArray_initWithBoostMatrix.m
//  iGANext
//
//  Created by 片ノ坂 卓磨 on 13/02/05.
//  Copyright (c) 2013年 片ノ坂 卓磨. All rights reserved.
//

#import "NSMutableArray+initWithBoostDoubleMatrix.h"

@implementation NSMutableArray (initWithBoostDoubleMatrix)
- (NSMutableArray *) initWithBoostArray:(boost::numeric::ublas::matrix<double> *)boostDoubleMatrix
{
  NSMutableArray *Result = [[NSMutableArray alloc] init];

  for (int x = 0, xterm = boostDoubleMatrix->size1(); x < xterm; ++x) {
    NSMutableArray *rowArr = [NSMutableArray array];
    for (int y = 0, yterm = boostDoubleMatrix->size2(); y < yterm; ++y) {
      [rowArr addObject:[NSNumber numberWithDouble:boostDoubleMatrix->at_element(x, y)]];
    }
    [Result addObject:rowArr];
  }
  
  return Result;
}
@end

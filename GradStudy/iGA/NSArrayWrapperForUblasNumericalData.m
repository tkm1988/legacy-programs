//
//  NSArrayWrapperForUblasNumericalData.m
//  iGA
//
//  Created by 片ノ坂 卓磨 on 12/09/01.
//  Copyright (c) 2012年 北海道大学大学院 情報科学研究科 複合情報学専攻 表現系工学研究室. All rights reserved.
//

#import "NSArrayWrapperForUblasNumericalData.h"

@implementation NSArrayWrapperForUblasNumericalData
+ (boost::numeric::ublas::vector<double>) transrateDoubleVector:(NSArray *)NSArrayObject{
  boost::numeric::ublas::vector<double> numericVector([NSArrayObject count]);
  
  for (int i = 0, iMax = static_cast<int>(numericVector.size()); i < iMax; ++i) {
    numericVector(i) = [[NSArrayObject objectAtIndex:i] doubleValue];
  }
  
  return numericVector;
}
+ (boost::numeric::ublas::matrix<double>) transrateDoubleMatrix:(NSArray *)NSArrayObject{
  boost::numeric::ublas::matrix<double> numericMatrix([NSArrayObject count], [[NSArrayObject objectAtIndex:0] count]);
  
  for (int x = 0, xMax = static_cast<int>(numericMatrix.size1()); x < xMax; ++x) {
     for (int y = 0, yMax = static_cast<int>(numericMatrix.size2()); y < yMax; ++y){
       numericMatrix(x, y) = [[[NSArrayObject objectAtIndex:x] objectAtIndex:y] doubleValue];
    }
  }
  
  return numericMatrix;
}
@end

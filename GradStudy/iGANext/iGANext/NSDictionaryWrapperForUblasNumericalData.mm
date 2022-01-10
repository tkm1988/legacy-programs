//
//  NSDictionaryWrapperForUblasNumericalData.m
//  iGANext
//
//  Created by 片ノ坂 卓磨 on 13/02/04.
//  Copyright (c) 2013年 片ノ坂 卓磨. All rights reserved.
//

#import "NSDictionaryWrapperForUblasNumericalData.h"

@implementation NSDictionaryWrapperForUblasNumericalData
+ (boost::numeric::ublas::vector<double>) transrateDoubleVector:(NSDictionary *)NSDictObject{
  int count = 0;
  boost::numeric::ublas::vector<double> numericVector([NSDictObject count]);
  
  for (id key in NSDictObject) {
    numericVector(++count) = [[NSDictObject objectForKey:key] doubleValue];
  }
  
  return numericVector;
}

+ (boost::numeric::ublas::matrix<double>) transrateDoubleMatrix:(NSArray *)NSArrayObject{
  //int x = 0;
  int y = 0;
  boost::numeric::ublas::matrix<double> numericMatrix([NSArrayObject count],
                                                      [[NSArrayObject objectAtIndex:0] count]);
  for (id obj in NSArrayObject) {
    //x = 0;
    for (id key in obj) {
      //numericMatrix(y, x++) = [[obj objectForKey:key] doubleValue];
      if ([key isEqual:@"Hue"]) {
        numericMatrix(y, 0) = [[obj objectForKey:key] doubleValue];
      } else if ([key isEqual:@"Value"]) {
        numericMatrix(y, 1) = [[obj objectForKey:key] doubleValue];
      } else if ([key isEqual:@"Saturation"]) {
        numericMatrix(y, 2) = [[obj objectForKey:key] doubleValue];
      } else if ([key isEqual:@"Collar"]) {
        numericMatrix(y, 3) = [[obj objectForKey:key] doubleValue];
      } else if ([key isEqual:@"Sleeve"]) {
        numericMatrix(y, 4) = [[obj objectForKey:key] doubleValue];
      } else if ([key isEqual:@"x"]) {
        numericMatrix(y, 5) = [[obj objectForKey:key] doubleValue];
      } else if ([key isEqual:@"y"]) {
        numericMatrix(y, 6) = [[obj objectForKey:key] doubleValue];
      } else {
        numericMatrix(y, 7) = [[obj objectForKey:key] doubleValue];
      }
    }
    ++y;
  }
  return numericMatrix;
}
@end

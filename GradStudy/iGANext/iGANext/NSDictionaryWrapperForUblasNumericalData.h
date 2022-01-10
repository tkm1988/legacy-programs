//
//  NSDictionaryWrapperForUblasNumericalData.h
//  iGANext
//
//  Created by 片ノ坂 卓磨 on 13/02/04.
//  Copyright (c) 2013年 片ノ坂 卓磨. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Foundation/NSDictionary.h>
#import <boost/numeric/ublas/vector.hpp>
#import <boost/numeric/ublas/matrix.hpp>

@interface NSDictionaryWrapperForUblasNumericalData : NSObject
+ (boost::numeric::ublas::vector<double>) transrateDoubleVector:(NSDictionary *)NSDictObject;
+ (boost::numeric::ublas::matrix<double>) transrateDoubleMatrix:(NSArray *)NSArrayObject;
@end

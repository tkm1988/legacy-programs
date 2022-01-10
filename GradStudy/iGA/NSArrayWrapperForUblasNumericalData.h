//
//  NSArrayWrapperForUblasNumericalData.h
//  iGA
//
//  Created by 片ノ坂 卓磨 on 12/09/01.
//  Copyright (c) 2012年 北海道大学大学院 情報科学研究科 複合情報学専攻 表現系工学研究室. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Foundation/NSArray.h>
#import <boost/numeric/ublas/vector.hpp>
#import <boost/numeric/ublas/matrix.hpp>

@interface NSArrayWrapperForUblasNumericalData
+ (boost::numeric::ublas::vector<double>) transrateDoubleVector:(NSArray *)NSArrayObject;
+ (boost::numeric::ublas::matrix<double>) transrateDoubleMatrix:(NSArray *)NSArrayObject;
@end

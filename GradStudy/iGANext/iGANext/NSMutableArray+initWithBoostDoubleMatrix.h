//
//  NSMutableArray+NSMutableArray_initWithBoostMatrix.h
//  iGANext
//
//  Created by 片ノ坂 卓磨 on 13/02/05.
//  Copyright (c) 2013年 片ノ坂 卓磨. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <boost/numeric/ublas/matrix.hpp>

@interface NSMutableArray (initWithBoostDoubleMatrix)
- (NSMutableArray *) initWithBoostArray:(boost::numeric::ublas::matrix<double> *)boostDoubleMatrix;
@end

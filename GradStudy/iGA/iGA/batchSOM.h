//
//  batchSOM.h
//  iGA
//
//  Created by 片ノ坂 卓磨 on 12/08/31.
//  Copyright (c) 2012年 北海道大学大学院 情報科学研究科 複合情報学専攻 表現系工学研究室. All rights reserved.
//
#ifndef iGA_batchSOM_h
#define iGA_batchSOM_h

#include <cmath>
#include <cfloat>
#include <ctime>
#include <climits>
#include <algorithm>
#include <list>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/random.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#import <boost/numeric/ublas/matrix_proxy.hpp>
#import "rnd_shuffle.h"

class BatchSOM:public som{
public:
  BatchSOM(int const, int const, dmat *);
  ~BatchSOM(void);
  void	ProdLearning(int const, bool const);
  void	NormLearning(int const, bool const);
};

#endif

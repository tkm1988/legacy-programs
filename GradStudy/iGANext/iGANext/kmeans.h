//
//  kmeans.h
//  iGA
//
//  Created by 片ノ坂 卓磨 on 12/09/01.
//  Copyright (c) 2012年 北海道大学大学院 情報科学研究科 複合情報学専攻 表現系工学研究室. All rights reserved.
//

#ifndef __iGA__kmeans__
#define __iGA__kmeans__

#import <list>
#import <vector>
#import <boost/numeric/ublas/io.hpp>
#import <boost/numeric/ublas/matrix.hpp>
#import <boost/numeric/ublas/matrix_proxy.hpp>
#import <boost/numeric/ublas/vector.hpp>

using namespace std;
using namespace boost;
using namespace boost::numeric::ublas;

typedef matrix<double> dmat;
typedef matrix_row<dmat> dmat_r;
typedef boost::numeric::ublas::vector<double> boost_dvect;
typedef list<double> dlist;
typedef std::vector<boost_dvect> stl_dvvect;

stl_dvvect kmeans(int , dmat*, int);
void choose_random_center(int, stl_dvvect*, dmat*);
void choose_smart_center(int, stl_dvvect*, dmat*);
void classification(int, stl_dvvect*, dmat*);
#endif /* defined(__iGA__kmeans__) */

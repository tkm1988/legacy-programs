//
//  som.h
//  iGA
//
//  Created by 片ノ坂 卓磨 on 12/08/31.
//  Copyright (c) 2012年 北海道大学大学院 情報科学研究科 複合情報学専攻 表現系工学研究室. All rights reserved.
//

#ifndef __iGA__som__
#define __iGA__som__

//#import "rnd_shuffle.h"
#import <cmath>
#import <cfloat>
#import <ctime>
#import <climits>
#import <algorithm>
#import <list>
#import <vector>
#import <boost/lexical_cast.hpp>
#import <boost/random.hpp>
#import <boost/numeric/ublas/io.hpp>
#import <boost/numeric/ublas/matrix.hpp>
#import <boost/numeric/ublas/vector.hpp>
#import <boost/numeric/ublas/matrix_proxy.hpp>


using namespace std;
using namespace boost;
using namespace boost::numeric::ublas;

typedef matrix<double>				dmat;
typedef boost::numeric::ublas::vector<double>	dvect;

template < typename T >

class Increment{
  T val_;
public:
  Increment( const T& init ) : val_( init ){}
  T operator()(){ return val_++; }
};

class som{
protected:
  dmat			*InputMatrix;
  dmat			 RefMatrix;
  list<double>           Error;
  std::vector<int>	 BrokenNeuronFlag;
  int			 ltime;
  int			 ltime_limit;
  int			 x;
  int			 y;
  double		 alpha;
  double		 sigma;
  bool			 w_normalize_flag;
  bool			 d_normalize_flag;
public:
  som(int const, int const, int const);
  ~som(void);
  void			setData(dmat *);
  void			setLtimeLimit(int);
  double		getAlpha(void);
  double		getSigma(void);
  dmat			getRefMatrix(void);
  dvect			getCoord(dvect);
  std::vector<int>	getBrokenNeuronFlag(void);
  void                  Normalization(dmat *);
  virtual void	     	ProdLearning(int const, double const, double const, bool const);
  virtual void 		NormLearning(int const, double const, double const, bool const);
  void			broken_neuron(double const);
};

#endif /* defined(__iGA__som__) */

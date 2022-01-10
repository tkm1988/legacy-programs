//
//  rnd_shuffle.h
//  iGA
//
//  Created by 片ノ坂 卓磨 on 12/08/31.
//  Copyright (c) 2012年 北海道大学大学院 情報科学研究科 複合情報学専攻 表現系工学研究室. All rights reserved.
//

#ifndef __iGA__rnd_shuffle__
#define __iGA__rnd_shuffle__

#import <iostream>
#import <ctime>

#if defined (nil)
#undef nil
#endif

#if defined (Nil)
#undef Nil
#endif

#define nil cpp_nil
#define Nil cpp_nil

#import <boost/random.hpp>

#undef nil
#undef Nil
#define nil NULL
#define Nil NULL

template < typename T >

class Increment{
  T val_;
public:
  Increment( const T& init ) : val_( init ){}
  T operator()(){ return val_++; }
};

class Random{
public:
	boost::mt19937 gen;
	boost::uniform_int<int> dst;
	boost::variate_generator< boost::mt19937, boost::uniform_int<int> > rand;
	
	Random( int num ):// call instance:
	gen( static_cast<unsigned long>(std::time(0)) ), dst( 0, num ), rand( gen, dst ) {}
	
  std::ptrdiff_t operator()(std::ptrdiff_t arg){
    return static_cast< std::ptrdiff_t >( rand() );
  }
};

class rnd_shuffle{
private:
	int num;
	std::vector<int> vctr;
public:
	rnd_shuffle(int);
	std::vector<int> getshuffled();
};
#endif /* defined(__iGA__rnd_shuffle__) */

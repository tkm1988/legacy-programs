//
//  DrawBaseColor.m
//  iGA
//
//  Created by 片ノ坂 卓磨 on 12/07/23.
//  Copyright (c) 2012 北海道大学大学院 情報科学研究科 複合情報学専攻 表現系工学研究室. All rights reserved.
//
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

#import "DrawBaseColor.h"

@implementation DrawBaseColor

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
    }
    return self;
}


// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
  boost::mt19937 gen(static_cast<unsigned long>(std::time(0)));
  boost::uniform_real<> dst(0, 1);
	boost::variate_generator<boost::mt19937, boost::uniform_real<> > random(gen, dst);

  CGContextRef context;
  for(int y = 0; y < 4; ++y){
    for(int x = 0; x < 5; ++x){
      context = UIGraphicsGetCurrentContext();
      double const Red = (double)random();
      double const Green = (double)random();
      double const Blue = (double)random();
      CGContextSetRGBFillColor(context, Red, Green, Blue, 1.0);
      CGRect r1 = CGRectMake(X_ORIGIN+(IMG_WIDTH*x), Y_ORIGIN+(IMG_HEIGHT*y), IMG_WIDTH, IMG_HEIGHT);
      CGContextAddRect(context, r1);
      CGContextFillPath(context);
      CGContextStrokePath(context);
    }
  }
}

@end

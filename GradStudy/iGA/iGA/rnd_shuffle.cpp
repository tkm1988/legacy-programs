//
//  rnd_shuffle.cpp
//  iGA
//
//  Created by 片ノ坂 卓磨 on 12/08/31.
//  Copyright (c) 2012年 北海道大学大学院 情報科学研究科 複合情報学専攻 表現系工学研究室. All rights reserved.
//

#include "rnd_shuffle.h"

rnd_shuffle::rnd_shuffle(int num){
	this->num = num;
  this->vctr.clear();
	Random rnd(num - 1);
  
	std::generate_n(std::back_inserter(vctr), num, Increment<int>(0));
	std::random_shuffle(vctr.begin(), vctr.end(), rnd);
}

std::vector<int> rnd_shuffle::getshuffled(){
	return vctr;
}
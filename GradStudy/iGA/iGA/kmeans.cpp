//
//  kmeans.cpp
//  iGA
//
//  Created by 片ノ坂 卓磨 on 12/09/01.
//  Copyright (c) 2012年 北海道大学大学院 情報科学研究科 複合情報学専攻 表現系工学研究室. All rights reserved.
//
#import <cstdlib>
#import <cfloat>
#import <cmath>
#import <map>
#import <iostream>
#import <boost/lexical_cast.hpp>
#import "rnd_shuffle.h"
#import "kmeans.h"

#define STANDARD_K_MEANS 1
#define K_MEANS_PLAS_PLAS 2

//k-means is prototype-based unsupervised classification algorithm.
//"k" is the variable that contains the number of representation point.
//"dm" is a prototype dataset.
stl_dvvect kmeans(int const k, dmat *dm, int mode){
	stl_dvvect center_of_cluster;
	
	switch(mode){
    case 1:choose_random_center(k, &center_of_cluster, dm);break;
    case 2:choose_smart_center(k, &center_of_cluster, dm);break;
    default:choose_random_center(k, &center_of_cluster, dm);break;
	}
  
	classification(k, &center_of_cluster, dm);
	
	return center_of_cluster;
}
void choose_random_center(int const k, stl_dvvect* center_of_cluster, dmat* const dm){
	//rnd_shuffle rs(dm->size1());
	//std::vector<int> rs_vector = rs.getshuffled();
  std::vector<int> rs_vector;
  std::generate_n(std::back_inserter(rs_vector), (int)dm->size1(), Increment<int>(0));
  std::random_shuffle(rs_vector.begin(), rs_vector.end());

	
	for(int i = 0; i < k; ++i){
		center_of_cluster->push_back(row(*dm, rs_vector.at(i)));
	}
}
void choose_smart_center(int const k, stl_dvvect* center_of_cluster, dmat* const dm){
	//rnd_shuffle rs(dm->size1());
	//std::vector<int> rs_vector = rs.getshuffled();
  std::vector<int> rs_vector;
  std::generate_n(std::back_inserter(rs_vector), (int)dm->size1(), Increment<int>(0));
  std::random_shuffle(rs_vector.begin(), rs_vector.end());
  
  center_of_cluster->push_back(row(*dm, rs_vector.at(0)));
  
  for(int i = 1; i < k; ++i){
    int target_index = 0;
    double probability = DBL_MIN;
    double sumD = 0.0, D[(int)dm->size1()];
    
    for(int j = 0; j < (int)dm->size1();++j){
      double min_dist = DBL_MAX;
      
      for(int num = 0; num < (int)center_of_cluster->size(); ++num){
        min_dist = fmin(min_dist, norm_2(center_of_cluster->at(num) - row(*dm, j)));
      }
      D[j] = min_dist;
      sumD += D[j] * D[j];
    }
    
    for(int j = 0; j < (int)dm->size1(); ++j){
      if(probability < fmax(probability, (D[j] * D[j]) / sumD)){
        probability = (D[j] * D[j]) / sumD ;
        target_index = j;
      }
    }
    
    if(target_index >= 0 && target_index < (int)dm->size1()){
      center_of_cluster->push_back(row(*dm, target_index));
    }else{
      cout << "error occured !\n" << endl;
    }
  }
}


void classification(int const k, stl_dvvect* center_of_cluster, dmat* const dm){
  bool change_class = false;
  map<int, int> index_class_map;
  
  //This part assign all datas to a nearest cluster.
  while(true){
    change_class = false;
    
    for(int i = 0; i < (int)dm->size1(); ++i){
      int class_num = -1;
      double min_dist = DBL_MAX;
      
      for(int j = 0; j <  k; ++j){
        if(fmin(min_dist, norm_2(center_of_cluster->at(j) - row(*dm, i))) < min_dist){
          min_dist = norm_2(center_of_cluster->at(j) - row(*dm, i));
          class_num = j;
        }
      }
      
      if(index_class_map.empty() || (unsigned int) index_class_map.size() < (unsigned int)dm->size1()){
        index_class_map.insert(map<int, int>::value_type(i, class_num));
        change_class = true;
      }else{
        if(index_class_map[i] != class_num){
          index_class_map[i] = class_num;
          change_class = true;
        }
      }
    }
    
    if(!change_class){
      break;
    }
		
    //Compute each center of clusters.
    center_of_cluster->clear();
    
    for(int i = 0; i < k; ++i){
      int sumNum = 0;
      boost_dvect sumC;
      sumC.resize(dm->size2());
      
      for(int j = 0; j < (int)dm->size1(); ++j){
        if(index_class_map[j] == i){
          ++sumNum;
          sumC += row(*dm, j);
        }
      }
      try{
        if(sumNum > 0){
          center_of_cluster->push_back(sumC / (double)sumNum);
        }else{
          center_of_cluster->push_back(center_of_cluster->at(0));
        }
      }catch(char *e){
        cout << "Exception [k-means.hpp] L210 - 214 : " << e << endl;
      }
    }	
  }
}
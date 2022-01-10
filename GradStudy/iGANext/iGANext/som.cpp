//
//  som.cpp
//  iGA
//
//  Created by 片ノ坂 卓磨 on 12/08/31.
//  Copyright (c) 2012年 北海道大学大学院 情報科学研究科 複合情報学専攻 表現系工学研究室. All rights reserved.
//

#include "som.h"

som::som(int const xl, int const yl, int const data_dim){
  InputMatrix = NULL;
  ltime	      = 0;
  ltime_limit = 0;
  x	      = xl;
  y	      = yl;
  alpha  = 0.0;
  sigma  = 0.0;
  w_normalize_flag = false;
  d_normalize_flag = false;
  RefMatrix.resize(x * y, data_dim);
  BrokenNeuronFlag.resize(x * y);
  
  mt19937	gen(static_cast<unsigned long>(time(0)));
  uniform_smallint<> dst(1, 10);
  variate_generator<mt19937, uniform_smallint<> > rand(gen, dst);
  
  for(int i = 0, i_max = x * y; i < i_max; ++i){
    for(int j = 0, j_max = data_dim; j < j_max; ++j){
      RefMatrix(i, j) = rand() * 0.01;
      BrokenNeuronFlag.push_back(0);
    }
  }
}

som::~som(){
}

void som::setData(dmat *Input){
  InputMatrix = Input;
}

void som::setLtimeLimit(int ltime_limit){
  som::ltime_limit = ltime_limit;
}

dmat som::getRefMatrix(){
  return RefMatrix;
}

double som::getAlpha(){
  return alpha;
}

double som::getSigma(){
  return sigma;
}

std::vector<int> som::getBrokenNeuronFlag(){
  return BrokenNeuronFlag;
}

void som::Normalization(dmat *Input){
  for(int i = 0, i_max = static_cast<int>(Input->size1()); i < i_max; ++i){
    row(*Input, i) = row(*Input, i) / norm_2(row(*Input, i));
  }
}

void som::ProdLearning(int const ltime, double const init_alpha, double const init_sigma, bool const trace_mode){
  ltime_limit = ltime;
  
  if(w_normalize_flag == false){
    som::Normalization(&RefMatrix);
    w_normalize_flag = true;
  }
  
  if(d_normalize_flag == false){
    som::Normalization(InputMatrix);
    d_normalize_flag = true;
  }
  
  for(int i = 0, i_max = (x * y); i < i_max; ++i){
    for(std::vector<int>::iterator it = BrokenNeuronFlag.begin(), it_end = BrokenNeuronFlag.end(); it != it_end; ++it, ++i){
      if(*it == 1){
        row(RefMatrix, i) = row(RefMatrix, i) * 0.0;
      }
    }
  }
  
  if(trace_mode == true){
    double		error_sum = 0.0;
    //rnd_shuffle		rs((int)InputMatrix->size1());
    //std::vector<int>	rsv	  = rs.getshuffled();
    std::vector<int> rsv;
    std::generate_n(std::back_inserter(rsv), (int)InputMatrix->size1(), Increment<int>(0));
    std::random_shuffle(rsv.begin(), rsv.end());
    
    alpha = init_alpha * pow(0.01 / init_alpha, static_cast<double>(som::ltime) / static_cast<double>(ltime_limit));
    sigma = init_sigma * pow(0.01 / init_sigma, static_cast<double>(som::ltime) / static_cast<double>(ltime_limit));
    
    for(std::vector<int>::iterator it  = rsv.begin(), it_end = rsv.end(); it != it_end; ++it){
      dvect     Rc(2);
      dvect	ProdResult	  = prod(RefMatrix, row(*InputMatrix, *it));
      int	max_element_index = max_element(ProdResult.begin(), ProdResult.end()).index();
      
      Rc(0) = max_element_index / x;
      Rc(1) = max_element_index % x;
      
      for(int i = 0, i_max = (x * y); i < i_max; ++i){
        dvect R(2);
        R(0)			     = i / x;
        R(1)			     = i % x;
        double			h    = alpha * exp(-1.0 * norm_2(Rc - R) / (2 * sigma * sigma));
        dvect            	work = row(RefMatrix, i) + h*row(*InputMatrix, *it);
        row(RefMatrix, i)	     = work / norm_2(work);
      }
      error_sum += norm_2(row(RefMatrix, max_element_index) - row(*InputMatrix, *it));
    }
    
    Error.push_back(error_sum / static_cast<double>(x * y));
    ++som::ltime;
  }else{
    while(som::ltime < ltime_limit){
      cout << som::ltime << endl;
      double		error_sum = 0.0;
      //rnd_shuffle		rs((int)InputMatrix->size1());
      //std::vector<int>	rsv	  = rs.getshuffled();
      std::vector<int> rsv;
      std::generate_n(std::back_inserter(rsv), (int)InputMatrix->size1(), Increment<int>(0));
      std::random_shuffle(rsv.begin(), rsv.end());

      
      alpha = init_alpha * pow(0.01 / init_alpha, static_cast<double>(som::ltime) / static_cast<double>(ltime_limit));
      sigma = init_sigma * pow(0.01 / init_sigma, static_cast<double>(som::ltime) / static_cast<double>(ltime_limit));
      
      for(std::vector<int>::iterator it  = rsv.begin(), it_end = rsv.end(); it != it_end; ++it){
        dvect Rc(2);
        dvect	ProdResult	  = prod(RefMatrix, row(*InputMatrix, *it));
        int	max_element_index = max_element(ProdResult.begin(), ProdResult.end()).index();
        
        Rc(0) = max_element_index / x;
        Rc(1) = max_element_index % x;
        
        for(int i = 0, i_max = (x * y); i < i_max; ++i){
          dvect			R(2);
          R(0)			     = i / x;
          R(1)			     = i % x;
          double		h    = alpha * exp(-1.0 * norm_2(Rc - R) / (2 * sigma * sigma));
          dvect                 work = row(RefMatrix, i) + h*row(*InputMatrix, *it);
          row(RefMatrix, i)	     = work / norm_2(work);
        }
        error_sum += norm_2(row(RefMatrix, max_element_index) - row(*InputMatrix, *it));
      }
      
      Error.push_back(error_sum / static_cast<double>(x * y));
      ++som::ltime;
    }
  }
}

void som::NormLearning(int const ltime, double const init_alpha, double const init_sigma, bool const trace_mode){
  if(ltime_limit == 0){
    ltime_limit = ltime;
  }
  
  if(w_normalize_flag == true){
    w_normalize_flag = false;
  }
  if(d_normalize_flag == true){
    d_normalize_flag = true;
  }
  
  for(int i = 0, i_max = (x * y); i < i_max; ++i){
    for(std::vector<int>::iterator it = BrokenNeuronFlag.begin(), it_end = BrokenNeuronFlag.end(); it != it_end; ++it, ++i){
      if(*it == 1){
        row(RefMatrix, i) = row(RefMatrix, i) * DBL_MAX;
      }
    }
  }
  
  if(trace_mode == true){
    double		error_sum = 0.0;
    //rnd_shuffle		rs((int)InputMatrix->size1());
    //std::vector<int>	rsv	  = rs.getshuffled();
    std::vector<int> rsv;
    std::generate_n(std::back_inserter(rsv), (int)InputMatrix->size1(), Increment<int>(0));
    std::random_shuffle(rsv.begin(), rsv.end());

    
    alpha = init_alpha * pow(0.01 / init_alpha, static_cast<double>(som::ltime) / static_cast<double>(ltime_limit));
    sigma = init_sigma * pow(0.01 / init_sigma, static_cast<double>(som::ltime) / static_cast<double>(ltime_limit));
    
    for(std::vector<int>::iterator it  = rsv.begin(), it_end = rsv.end(); it != it_end; ++it){
      dvect Rc(2), NormVector(x * y);
      int	min_element_index = 0;
      
      for(int i = 0, i_max = (x * y); i < i_max; ++i){
        NormVector(i) = norm_2(row(RefMatrix, i) - row(*InputMatrix, *it));
      }
      min_element_index = min_element(NormVector.begin(), NormVector.end()).index();
      Rc(0) = min_element_index / x;
      Rc(1) = min_element_index % x;
      
      for(int i = 0, i_max = (x * y); i < i_max; ++i){
        dvect R(2);
        R(0)		  = i / x;
        R(1)		  = i % x;
        double	h	  = alpha * exp(-1.0 * norm_2(Rc - R) / (2 * sigma * sigma));
        row(RefMatrix, i) = row(RefMatrix, i) + h*(row(*InputMatrix, *it) - row(RefMatrix, i));
      }
      error_sum += norm_2(row(RefMatrix, min_element_index) - row(*InputMatrix, *it));
    }
    
    Error.push_back(error_sum / static_cast<double>(x * y));
    ++som::ltime;
  }else{
    while(som::ltime < ltime_limit){
      double		error_sum = 0.0;
      //rnd_shuffle		rs((int)InputMatrix->size1());
      //std::vector<int>	rsv	  = rs.getshuffled();
      std::vector<int> rsv;
      std::generate_n(std::back_inserter(rsv), (int)InputMatrix->size1(), Increment<int>(0));
      std::random_shuffle(rsv.begin(), rsv.end());

      
      alpha = init_alpha * pow(0.01 / init_alpha, static_cast<double>(som::ltime) / static_cast<double>(ltime_limit));
      sigma = init_sigma * pow(0.01 / init_sigma, static_cast<double>(som::ltime) / static_cast<double>(ltime_limit));
      
      for(std::vector<int>::iterator it  = rsv.begin(), it_end = rsv.end(); it != it_end; ++it){
        dvect Rc(2), NormVector(x * y);
        int	min_element_index = 0;
        
        for(int i = 0, i_max = (x * y); i < i_max; ++i){
          NormVector(i) = norm_2(row(RefMatrix, i) - row(*InputMatrix, *it));
        }
        min_element_index = min_element(NormVector.begin(), NormVector.end()).index();
        Rc(0) = min_element_index / x;
        Rc(1) = min_element_index % x;
        
        for(int i = 0, i_max = (x * y); i < i_max; ++i){
          dvect	R(2);
          R(0)		     = i / x;
          R(1)		     = i % x;
          double	h    = alpha * exp(-1.0 * norm_2(Rc - R) / (2 * sigma * sigma));
          row(RefMatrix, i) = row(RefMatrix, i) + h*(row(*InputMatrix, *it) - row(RefMatrix, i));
        }
        error_sum += norm_2(row(RefMatrix, min_element_index) - row(*InputMatrix, *it));
      }
      
      Error.push_back(error_sum / static_cast<double>(x * y));
      ++som::ltime;
    }
  }
}

void som::broken_neuron(double const rate){
  //rnd_shuffle		rs((int)InputMatrix->size1());
  //std::vector<int>	rsv	  = rs.getshuffled();
  std::vector<int> rsv;
  std::generate_n(std::back_inserter(rsv), (int)InputMatrix->size1(), Increment<int>(0));
  std::random_shuffle(rsv.begin(), rsv.end());

  
  for(int i = 0, i_max = static_cast<int>(static_cast<double>(x * y)/100.0 * rate); i < i_max; ++i){
    for(std::vector<int>::iterator it = rsv.begin(), it_end = rsv.end(); (it != it_end)&&(i < i_max); ++it, ++i){
      BrokenNeuronFlag.at(*it) = 1;
    }
  }
}

dvect som::getCoord(dvect inputVector){
  int min_index = 0;
  dvect coord(2);
  dvect vnorm((int)RefMatrix.size1());
  
  for(int i = 0; i < (x * y); ++i){
    vnorm(i) = norm_2(inputVector - row(RefMatrix, i));
  }
  
  min_index = min_element(vnorm.begin(), vnorm.end()).index();
  
  coord(0) = min_index - floor(static_cast<double>(min_index) / static_cast<double>(x)) * y;
  coord(1) = floor(static_cast<double>(min_index) / static_cast<double>(x));
  
  return coord;
}


//
//  batchSOM.cpp
//  iGA
//
//  Created by 片ノ坂 卓磨 on 12/08/31.
//  Copyright (c) 2012年 北海道大学大学院 情報科学研究科 複合情報学専攻 表現系工学研究室. All rights reserved.
//

#include "batchSOM.h"

BatchSOM::BatchSOM(int const xl, int const yl, dmat *Data):som::som(xl, yl, static_cast<int>(Data->size2())){
  som::InputMatrix = Data;
  rnd_shuffle			rs(static_cast<int>(som::InputMatrix->size1()));
  std::vector<int>		rsv = rs.getshuffled();
  
  for(int i = 0, i_max = som::x * som::y; i < i_max; ++i){
    for(std::vector<int>::iterator it = rsv.begin(), it_end = rsv.end(); (it != it_end)&&(i < i_max); ++it, ++i){
      row(som::RefMatrix, i) = row(*som::InputMatrix, *it);
      BrokenNeuronFlag.push_back(0);
    }
  }
}

BatchSOM::~BatchSOM(){
}

void BatchSOM::ProdLearning(int const ltime, bool const trace_mode){
  som::ltime_limit = ltime;
  
  if(som::w_normalize_flag == false){
    som::Normalization(&(som::RefMatrix));
    som::w_normalize_flag = true;
  }
  if(som::d_normalize_flag == false){
    som::Normalization(som::InputMatrix);
    som::d_normalize_flag = true;
  }
  
  for(int i = 0, i_max = (som::x * som::y); i < i_max; ++i){
    for(std::vector<int>::iterator it = som::BrokenNeuronFlag.begin(), it_end = som::BrokenNeuronFlag.end(); it != it_end; ++it, ++i){
      if(*it == 1){
        row(som::RefMatrix, i) = row(som::RefMatrix, i) * 0.0;
      }
    }
  }
  
  if(trace_mode == true){
    int                 *choose_counter = new int[som::x * som::y];
    double		error_sum      = 0.0;
    dmat SumLearning(static_cast<int>(som::InputMatrix->size1()), static_cast<int>(som::InputMatrix->size2()));
    SumLearning.clear();
    
    for(int i = 0, i_max = static_cast<int>(som::InputMatrix->size1()); i< i_max; ++i){
      dvect	ProdResult	  = prod(RefMatrix, row(*som::InputMatrix, i));
      int	max_element_index = max_element(ProdResult.begin(), ProdResult.end()).index();
      
      row(SumLearning, max_element_index) = row(SumLearning, max_element_index) + row(*som::InputMatrix, max_element_index);
      ++choose_counter[max_element_index];
      
      error_sum += norm_2(row(som::RefMatrix, max_element_index) - row(*som::InputMatrix, i));
    }
    
    for(int i = 0, i_max = (som::x * som::y); i < i_max; ++i){
      row(som::RefMatrix, i) = (row(som::RefMatrix, i) + row(SumLearning, i)) / (choose_counter[i] + 1);
    }
    
    som::Error.push_back(error_sum / static_cast<double>(som::x * som::y));
    ++som::ltime;
  }else{
    while(som::ltime < som::ltime_limit){
      int                 *choose_counter = new int[som::x * som::y];
      double		  error_sum      = 0.0;
      dmat SumLearning(static_cast<int>(som::InputMatrix->size1()), static_cast<int>(som::InputMatrix->size2()));
      SumLearning.clear();
      
      for(int i = 0, i_max = static_cast<int>(som::InputMatrix->size1()); i< i_max; ++i){
        dvect	ProdResult	  = prod(RefMatrix, row(*som::InputMatrix, i));
        int	max_element_index = max_element(ProdResult.begin(), ProdResult.end()).index();
        
        row(SumLearning, max_element_index) = row(SumLearning, max_element_index) + row(*som::InputMatrix, max_element_index);
        ++choose_counter[max_element_index];
        
        error_sum += norm_2(row(som::RefMatrix, max_element_index) - row(*som::InputMatrix, i));
      }
      
      for(int i = 0, i_max = (som::x * som::y); i < i_max; ++i){
        row(RefMatrix, i) = (row(som::RefMatrix, i) + row(SumLearning, i)) / (choose_counter[i] + 1);
      }
      
      som::Error.push_back(error_sum / static_cast<double>(som::x * som::y));
      ++som::ltime;
    }
  }
}

void BatchSOM::NormLearning(int const ltime, bool const trace_mode){
  if(ltime_limit == 0){
    som::ltime_limit = ltime;
  }
  
  if(som::w_normalize_flag == true){
    som::w_normalize_flag = false;
  }
  if(som::d_normalize_flag == true){
    som::d_normalize_flag = true;
  }
  
  for(int i = 0, i_max = (som::x * som::y); i < i_max; ++i){
    for(std::vector<int>::iterator it = som::BrokenNeuronFlag.begin(), it_end = som::BrokenNeuronFlag.end(); it != it_end; ++it, ++i){
      if(*it == 1){
        row(som::RefMatrix, i) = row(som::RefMatrix, i) * DBL_MAX;
      }
    }
  }
  
  if(trace_mode == true){
    int                 *choose_counter = new int[som::x *som:: y];
    double		error_sum      = 0.0;
    dmat SumLearning(static_cast<int>(som::InputMatrix->size1()), static_cast<int>(som::InputMatrix->size2()));
    SumLearning.clear();
    
    for(int i = 0, i_max = static_cast<int>(som::InputMatrix->size1()); i < i_max; ++i){
      dvect NormVector(som::x * som::y);
      int   min_element_index = 0;
      
      for(int j = 0, j_max = (som::x * som::y); j < j_max; ++j){
        NormVector(j) = norm_2(row(som::RefMatrix, j) - row(*som::InputMatrix, j));
      }
      min_element_index = min_element(NormVector.begin(), NormVector.end()).index();
      
      row(SumLearning, min_element_index) = row(SumLearning, min_element_index) + row(*som::InputMatrix, min_element_index);
      ++choose_counter[min_element_index];
      
      error_sum += norm_2(row(som::RefMatrix, min_element_index) - row(*som::InputMatrix, i));
    }
    
    for(int i = 0, i_max = (som::x * som::y); i < i_max; ++i){
      row(som::RefMatrix, i) = (row(som::RefMatrix, i) + row(SumLearning, i)) / (choose_counter[i] + 1);
    }
    
    Error.push_back(error_sum / static_cast<double>(som::x * som::y));
    ++som::ltime;
    delete [] choose_counter;
  }else{
    while(som::ltime < som::ltime_limit){
      int			*choose_counter = new int[som::x * som::y];
      double			error_sum      = 0.0;
      dmat SumLearning(static_cast<int>(som::InputMatrix->size1()), static_cast<int>(som::InputMatrix->size2()));
      SumLearning.clear();
      
      for(int i = 0, i_max = static_cast<int>(som::InputMatrix->size1()); i< i_max; ++i){
        dvect	NormVector(som::x * som::y);
        int	min_element_index = 0;
        
        for(int j = 0, j_max = (som::x * som::y); j < j_max; ++j){
          NormVector(j) = norm_2(row(som::RefMatrix, j) - row(*som::InputMatrix, j));
        }
        min_element_index = min_element(NormVector.begin(), NormVector.end()).index();
        
        row(SumLearning, min_element_index) = row(SumLearning, min_element_index) + row(*som::InputMatrix, min_element_index);
        ++choose_counter[min_element_index];
        
        error_sum += norm_2(row(som::RefMatrix, min_element_index) - row(*som::InputMatrix, i));
      }
      
      for(int i = 0, i_max = (x * som::y); i < i_max; ++i){
        row(som::RefMatrix, i) = (row(som::RefMatrix, i) + row(SumLearning, i)) / (choose_counter[i] + 1);
      }
      
      som::Error.push_back(error_sum / static_cast<double>(som::x * som::y));
      ++som::ltime;
      delete [] choose_counter;
    }
  }   
}
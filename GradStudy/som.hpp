#pragma once

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
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include "rnd_shuffle.h"

using namespace std;
using namespace boost;
using namespace boost::numeric::ublas;

typedef matrix<double>				dmat;
typedef boost::numeric::ublas::vector<double>	dvect;

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
    rnd_shuffle		rs(x * y);
    std::vector<int>	rsv	  = rs.getshuffled();

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
      double		error_sum = 0.0;
      rnd_shuffle		rs(x * y);
      std::vector<int>	rsv	  = rs.getshuffled();
      
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
    rnd_shuffle		rs(x * y);
    std::vector<int>	rsv	  = rs.getshuffled();

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
      rnd_shuffle		rs(x * y);
      std::vector<int>	rsv	  = rs.getshuffled();
      
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
  rnd_shuffle			rs(x * y);
  std::vector<int>		rsv = rs.getshuffled();
  
  for(int i = 0, i_max = static_cast<int>(static_cast<double>(x * y)/100.0 * rate); i < i_max; ++i){
    for(std::vector<int>::iterator it = rsv.begin(), it_end = rsv.end(); (it != it_end)&&(i < i_max); ++it, ++i){
      BrokenNeuronFlag.at(*it) = 1;
    }
  }
}

//
//      Batch SOM
//

class BatchSOM:public som{
public:
  BatchSOM(int const, int const, dmat *);
  ~BatchSOM(void);
  void	ProdLearning(int const, bool const);
  void	NormLearning(int const, bool const);  
};

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
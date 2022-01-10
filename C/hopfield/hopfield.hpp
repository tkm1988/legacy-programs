#pragma once

#include <ctime>
#include <algorithm>
#include <list>
#include <vector>
#include <boost/array.hpp>
#include <boost/random.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/symmetric.hpp>
#include "rnd_shuffle.h"

using namespace std;
using namespace boost;
using namespace boost::numeric::ublas;

typedef boost::numeric::ublas::vector<double> dvect;
typedef matrix<double> dmat;
typedef symmetric_matrix<double> sym_dmat;

class hopfield{
private:
  std::vector<int> state;
  dvect vstate;
  //dmat weight;
  sym_dmat weight;
public:
  hopfield(void);
  hopfield(int const *, int const);
  hopfield(int const);
  void setState(int const *);
  void setNum(int const);
  void setWeight(std::vector<int *> const *);
  void setWeight(std::vector<int *> const *, double const);
  list<double> recognize(int const);
  list<double> recognize(int const, int const *);
  std::vector<int> getState(void);
};

hopfield::hopfield(){
  state.clear();
  vstate.clear();
  weight.clear();
}
hopfield::hopfield(int const *init_state, int const num){
  vstate.resize(num);
  vstate.clear();

  for(int i = 0; i < num; ++i){
    state.push_back(init_state[i]);
    vstate(i) = init_state[i];
  }
  weight.resize(num);
  weight.clear();
}
hopfield::hopfield(int const num){
  vstate.resize(num);
  mt19937 gen((unsigned long)time(0));
  uniform_smallint<> dst(0, 1);
  variate_generator<mt19937 &, uniform_smallint<> >rand(gen, dst);
  for(int i = 0; i < num; ++i){
    state.push_back(rand());
    vstate(i) = state.at(i);
  }
  weight.resize(num);
  weight.clear();
}
void hopfield::setNum(int const num){
  vstate.resize(num);
  mt19937 gen((unsigned long)time(0));
  uniform_smallint<> dst(0, 1);
  variate_generator<mt19937 &, uniform_smallint<> >rand(gen, dst);
  for(int i = 0; i < num; ++i){
    state.push_back(rand());
    vstate(i) = state.at(i);
  }
  weight.resize(num);
  weight.clear();
}
void hopfield::setWeight(std::vector<int *> const *input){
  weight.clear();
  if((int)weight.size1() == 0 && (int)weight.size2() == 0){
    throw "Please set up numbers of neuron.";
  }

  for(int i = 0, offset = 0; i < (int)weight.size1(); ++i, ++offset){
    for(int j = offset; j < (int)weight.size2(); ++j){
      if(i == j){
	weight(i, j) = 0;
      }else{
	for(int k = 0; k < (int)input->size(); ++k){
	  weight(i, j) += (2 * input->at(k)[i] - 1) * (2 * input->at(k)[j] - 1);
	}
      }
    }
  }
}
void hopfield::setWeight(std::vector<int *> const *input, double const breakage_rate){
  rnd_shuffle rs((int)weight.size1());
  std::vector<int> rsv = rs.getshuffled();
  rsv.resize((int)((double)weight.size1() * 0.01 * breakage_rate));

  weight.clear();
  if((int)weight.size1() == 0 && (int)weight.size2() == 0){
    throw "Please set up numbers of neuron.";
  }

  for(int i = 0, offset = 0; i < (int)weight.size1(); ++i, ++offset){
    for(int j = offset; j < (int)weight.size2(); ++j){
      if((i == j) || count(rsv.begin(), rsv.end(), i) > 0){
	weight(i, j) = 0;
      }else{
	for(int k = 0; k < (int)input->size(); ++k){
	  weight(i, j) += (2 * input->at(k)[i] - 1) * (2 * input->at(k)[j] - 1);
	}
      }
    }
  }
}
void hopfield::setState(int const *init_state){
  state.clear();
  vstate.clear();
  try{
    for(int i = 0; i < (int)weight.size1(); ++i){
      state.push_back(init_state[i]);
      vstate(i) = init_state[i];
    }
  }catch(char *e){
    throw e;
  }
}
list<double> hopfield::recognize(int const lt){
  int t = 0;
  list<double> energy;
  mt19937 gen((unsigned long)time(0));
  uniform_smallint<> dst(0, (int)(weight.size1()) - 1);
  variate_generator<mt19937 &, uniform_smallint<> >rand(gen, dst);
  
  while(t < lt){
    int i = rand();
    double sum = 0.0;
    double cumulo_energy = 0.0;
    
    sum = (double)inner_prod(row(weight, i), vstate);

    if(sum >= 0.0){
      state.at(i) = 1;
      vstate(i) = 1;
    }else if(sum < 0.0){
      state.at(i) = 0;
      vstate(i) = 0;
    }

    for(i = 0; i < (int)(state.size()); ++i){
      cumulo_energy += (double)(vstate(i) * inner_prod(row(weight, i), vstate));
    }
    energy.push_back(-0.5 * cumulo_energy);
    ++t;
  }
  
  return energy;
}
list<double> hopfield::recognize(int const lt, int const *init_state){
  int t = 0;
  list<double> energy;
  mt19937 gen((unsigned long)time(0));
  uniform_smallint<> dst(0, (int)(weight.size1()) - 1);
  variate_generator<mt19937 &, uniform_smallint<> >rand(gen, dst);

  try{
    for(int i = 0; i < (int)(weight.size1()); ++i){
      state.push_back(init_state[i]);
      vstate(i) = init_state[i];
    }
  }catch(char *e){
    throw e;
  }

  while(t < lt){
    int i = rand();
    double sum = 0.0;
    double cumulo_energy = 0.0;
    
    sum = inner_prod(row(weight, i), vstate);

    if(sum > 0){
      state.at(i) = 1;
      vstate(i) = 1;
    }else if(sum < 0){
      state.at(i) = 0;
      vstate(i) = 0;
    }else{
      state.at(i) = state.at(i);
      vstate(i) = vstate(i);
    }

    for(i = 0; i < (int)(state.size()); ++i){
      cumulo_energy += (double)inner_prod(row(weight, i), (vstate(i) * vstate));
    }
    energy.push_back(-0.5 * cumulo_energy);
    ++t;
  }

  return energy;
}

std::vector<int> hopfield::getState(){
  return state;
}

#pragma once
#include <iostream>
#include <algorithm>
#include <list>
#include <vector>
#include <cmath>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include "neuron.h"
#include "som.hpp"

using namespace std;
using namespace boost;

typedef std::vector<neuron> nvect;
typedef list<double> dlist;
typedef numeric::ublas::vector<double> dvect;
typedef numeric::ublas::matrix<double> dmat;


class mip{
private:
  nvect nv;
  som *s;
  double sigmoid(double param, double alpha){return (1.0 / (1.0 + exp(-alpha * param)));}
  double delta(double param){return ((param>=0)?1.0:0.0);}  
 public:
  mip(void);
  mip(som *);
  void setSOM(som *);
  dlist learning(dmat const *, dmat const *, int const, double);
  dvect output(dvect *);
};

mip::mip(){
  s = NULL;
  nv.clear();
}
mip::mip(som *instance){
  s = instance;
  nv.clear();
}
void mip::setSOM(som *instance){
  s = instance;
  nv.clear();
}
dlist mip::learning(dmat const *x, dmat const *t, int const lt, double eta){
  int time = 0;
  int som_size = 0;
  int bnvector_size = 0;
  dlist error;
  nvect tmp;

  if(s == NULL){
    throw "Exception [mip.hpp] : Please set up the hidden layer.";
  }

  som_size = s->size();
  bnvector_size = (int)s->getBNIndexList().size();
  nv.clear();
  nv.reserve((int)t->size2());
  tmp.reserve((int)nv.size());
  for(int i = 0; i < (int)t->size2(); ++i){
    neuron n = neuron(s->size(), i);
    nv.push_back(n);
  }
  
  //Learning Part
  while(time < lt){
    double cumulo_error = 0.0;

    //Deep Copy
    tmp.clear();
    for(int i = 0, i_max = (int)nv.size(); i < i_max; ++i){
      tmp.push_back(nv.at(i));
    }

    for(int i = 0, i_max = (int)x->size1(); i < i_max; ++i){
      nvect::iterator nv_it = nv.begin();
      nvect::iterator tmp_it = tmp.begin();
      dvect coord = s->getCoord(row(*x, i));

      for(int j = 0, j_max = (int)t->size2(); j < j_max; ++j){
	//Choose a Unit
	double w = nv_it->getWeight((int)(coord(0) + coord(1) * sqrt((double)som_size)));
	double w_tmp = tmp_it->getWeight((int)(coord(0) + coord(1) * sqrt((double)som_size))); 
	
	//Feed Input Data to a chosen Unit
	//double result = sigmoid(w_tmp, 500);
	double result = 0.0;
	if(bnvector_size == som_size){
	  result = 1;
	}else{
	  result = delta(w_tmp);
	  //Renewal
	  if(result != (*t)(i, j)){
	    nv_it->setWeight(w  + eta * ((*t)(i, j) - result) , (int)(coord(0) + coord(1) * sqrt((double)som_size)));
	  }
	}

	cumulo_error += ((*t)(i, j) - result)*((*t)(i, j) - result);
	
	++nv_it;
	++tmp_it;
      }
    }

    error.push_back(cumulo_error / ((double)nv.size() * (double)t->size1()));
    eta = eta * exp(-75.0 * eta * ((double)time/(double)lt));
    ++time;
  }

  return error;
}

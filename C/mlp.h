#pragma once

#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <cmath>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include "neuron.h"

using namespace std;
using namespace boost;

typedef std::vector<neuron> nvect;
typedef list<double> dlist;
typedef numeric::ublas::vector<double> dvect;
typedef numeric::ublas::matrix<double> dmat;
typedef numeric::ublas::matrix_row<dmat> dmat_r;

class mlp{
 private:
  int hlayer_num;
  int hneuron_num;
  int oneuron_num;
  nvect vneuron_h;
  nvect vneuron_o;
  dmat input_matrix;
  dmat teacher;
  double sigmoid(double param, double alpha){return (1 / (1 + exp(-alpha * param)));}
  
 public:
  mlp(dmat *, dmat *, int, int, int);
  dlist learning(int, double);
  dvect recognize(dmat_r);
};

mlp::mlp(dmat *input_matrix, dmat *teacher, int const hlayer_num, int const hidden_neuron_num, int const out_neuron_num){
  this->input_matrix = *input_matrix;
  this->teacher = *teacher;
  this->hlayer_num = hlayer_num;
  this->hneuron_num = hidden_neuron_num;
  this->oneuron_num = out_neuron_num;
  
  for(int i = 0; i < hlayer_num; ++i){
    for(int j = 0; j < hneuron_num; ++j){
      neuron n = neuron((int)this->input_matrix.size2(), j + i * (int)this->input_matrix.size2());
      vneuron_h.push_back(n);
    }
  }
  
  for(int i = 0; i < oneuron_num; ++i){
    neuron n = neuron(hneuron_num, i);
    vneuron_o.push_back(n);
  }
}

dlist mlp::learning(int learning_time, double init_eta){
  double const alpha = 1.0;
  double eta = init_eta;
  dlist error;
  
  for(int t = 0; t < learning_time; ++t){
    //std::cout << t + 1 << std::endl;
    double cumulo_error = 0.0;
    
    for(int i = 0; i < (int)input_matrix.size1(); ++i){
      dmat_r feature_input_row(input_matrix, i);
      dmat_r feature_teacher_row(teacher, i);
      
      //Feed a Data
      //Input -> Hidden
      for(nvect::iterator it = vneuron_h.begin(); it !=  vneuron_h.end(); ++it){
	double temp = 0.0;
	for(int j = 0; j < (int)input_matrix.size2(); ++j){
	  temp += it->getWeight(j) * feature_input_row(j);
	}
	it->setNet(temp);
      }
      
      //Hidden -> Out
      for(nvect::iterator it = vneuron_o.begin(); it !=  vneuron_o.end(); ++it){
	double temp = 0.0;
	nvect::iterator hidden = vneuron_h.begin();
	
	for(int j = 0; j < hneuron_num; ++j, ++hidden){
	  temp += it->getWeight(j) * sigmoid(hidden->getNet() , alpha);
	}
	it->setNet(temp);
	
	cumulo_error += (feature_teacher_row(it->getCoord()) - sigmoid(temp, alpha)) * (feature_teacher_row(it->getCoord()) - sigmoid(temp, alpha));
      }
      
      //Renewal
      //Out -> Hidden
      for(nvect::iterator it = vneuron_o.begin(); it !=  vneuron_o.end(); ++it){
	nvect::iterator hidden = vneuron_h.begin();
	it->setSigma(-1.0 * (feature_teacher_row(it->getCoord()) - sigmoid(it->getNet(), alpha)) * sigmoid(it->getNet(), alpha) * (1.0 - sigmoid(it->getNet(), alpha)));
	
	for(int j = 0; j < hneuron_num; ++j, ++hidden){
	  it->setWeight(it->getWeight(j) - eta * it->getSigma() * sigmoid(hidden->getNet(), alpha), j);
	}
      }
      
      //Hidden -> Input
      for(nvect::iterator it = vneuron_h.begin(); it != vneuron_h.end(); ++it){
	double sum = 0.0;
	
	for(nvect::iterator out = vneuron_o.begin(); out != vneuron_o.end(); ++out){
	  sum += out->getSigma() * out->getWeight(it->getCoord());
	}
	
	for(int j = 0; j < (int)input_matrix.size2(); ++j){
	  it->setWeight(it->getWeight(j) - eta * sum * sigmoid(it->getNet(), alpha) * (1.0 - sigmoid(it->getNet(), alpha)) * feature_input_row(j), j);
	}
      }
    }
    error.push_back(cumulo_error / ((double)input_matrix.size1() * vneuron_o.size()));
    //eta = init_eta * (1.0 - ((double)(t + 1) / learning_time));
    eta = exp(-20.0 * init_eta * ((double)(t + 1) / learning_time));
  }
  
  return error;
}

dvect mlp::recognize(dmat_r input){
  const double alpha = 50.0;
  dvect out(vneuron_o.size());
  
  for(nvect::iterator it = vneuron_h.begin(); it !=  vneuron_h.end(); ++it){
    double sum = 0.0;
    for(int j = 0; j < (int)input_matrix.size2(); ++j){
      sum += it->getWeight(j) * input(j);
    }
    it->setNet(sum);
  }
  
  for(nvect::iterator it = vneuron_o.begin(); it !=  vneuron_o.end(); ++it){
    double sum = 0.0;
    nvect::iterator hidden = vneuron_h.begin();
    for(int j = 0; j < (int)input_matrix.size2(); ++j, ++hidden){
      sum += it->getWeight(j) * sigmoid(hidden->getNet() , alpha);
    }
    out(it->getCoord()) = sigmoid(sum, alpha);
  }
  
  return out;
}

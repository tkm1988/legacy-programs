#pragma once
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <cmath>
#include <map>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include "data.h"
#include "neuron.h"
#include "som.hpp"

using namespace std;
using namespace boost;

typedef std::vector<neuron> nvect;
typedef list<double> dlist;
typedef numeric::ublas::vector<double> dvect;
typedef numeric::ublas::matrix<double> dmat;
typedef numeric::ublas::matrix_row<dmat> dmat_r;


class perceptron{
private:
  int wakeup_cnt;//temp menber variable
  nvect nv;
  dmat inputVector;
  dmat teacher;
  som s;
  double sigmoid(double param, double alpha){return (1 / (1 + exp(-alpha * param)));}
  
 public:
  perceptron(dmat *, dmat, int);
  dlist som_learning(int, int, int, double, double);
  dvect output(dmat_r);
};

perceptron::perceptron(dmat *inputMatrix, dmat teacher, int outNeuronNum){
  wakeup_cnt = 0;
  this->inputMatrix = *inputMatrix;
  this->teacher = teacher;
  s = som(inputMatrix, &teacher, 2);
  for(int i = 0; i < outNeuronNum; ++i){
    neuron n = neuron(s.size(), i);
    nv.push_back(n);
  }
}

dlist perceptron::som_learning(int learning_time, int som_learning_time, int dimension, double perceptron_eta, double init_eta_som){
  //Const
  int const som_neuron_num = 100;
  string const fname = "iris";
  string const fpath_map = "(mip, somlt=" + lexical_cast<string>(som_learning_time) + ", perceptronlt=" \
    + lexical_cast<string>(learning_time) + ")" + fname;
  
  int t = 0;
  double eta = perceptron_eta;
  rnd_shuffle rs(som_neuron_num);
  std::vector<int> rsv = rs.getshuffled();
  std::vector<int>::iterator rsv_it = rsv.begin();
  dmat w_coord(1, 2);
  dlist error, error_som;
  error_som = s.Learning(&inputMatrix, som_learning_time, dimension, init_eta_som);

  if(wakeup_cnt <= 0){
    s.setHash();
    if(s.write_map(fpath_map, inputMatrix, s, s.getHash())){}
    ++wakeup_cnt;
  }
  
  //Learning Part
  cout << "Start to learn (Perceptron)" << endl;
  while(t < learning_time){
    double cumulo_error = 0.0;
    
    for(int i = 0; i < (int)inputMatrix.size1(); ++i){
      nvect::iterator nv_it = nv.begin();
      dmat_r inputMatrix_row(inputMatrix, i);
      dmat coord = s.getCoord(inputMatrix_row);
      
      for(int j = 0; j < (int)teacher.size2(); ++j){
	//Choose a Unit
	double w = nv_it->getWeight((int)(coord(0, 0) + coord(0,1) * sqrt((double)s.size())));
	
	//Feed Input Data to a chosen Unit
	double result = sigmoid(w, 500);

	//Break neurons
	/*
	for(int cnt = 0; cnt < som_neuron_num/100 * 50; ++cnt, ++rsv_it){
	  if((int)(coord(0, 0) + coord(0,1) * sqrt((double)s.size())) == *rsv_it){
	    result = 1.0;
	    break;
	  }
	}
	*/
	rsv_it = rsv.begin();
	
	//Renewal
	if(result != teacher(i, j)){
	  nv_it->setWeight(w  + eta * (teacher(i, j) - result) , (int)(coord(0, 0) + coord(0,1) * sqrt((double)s.size())));
	  cumulo_error = cumulo_error + (teacher(i, j) - result)*(teacher(i, j) - result);
	}
	++nv_it;
      }
    }
    error.push_back(cumulo_error / (nv.size() * inputMatrix.size1()));
    eta = perceptron_eta * exp(-75 * perceptron_eta* ((double)t/(double)learning_time));
    if(cumulo_error / (nv.size() * inputMatrix.size1()) == 0.0){break;}
    ++t;
  }
  cout << "Have finished to leran (Perceptron)" << endl;

  return error;
}

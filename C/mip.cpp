#include <cstdlib>
#include <cfloat>
#include <cmath>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include "data.h"
#include "som.hpp"
#include "perceptron.h"
#include "rnd_shuffle.h"

using namespace std;
using namespace boost;
using namespace boost::numeric::ublas;

typedef matrix<double> dmat;
typedef boost::numeric::ublas::vector<double> dvect;
typedef list<double> dlist;

int main(){
  //Const
  int const learning_time = 500;
  int const som_learning_time = 2;
  int const dimension = 2;
  int const max_sample = 100;
  double const p_eta = 0.1;
  double const s_eta = 0.1;
  string const fname = "iris";
  string const extension = ".dat";
  string const fpath = "dataset/" + fname + extension;
  string const fname_err = "error/(mip somlt=" + lexical_cast<string>(som_learning_time) + ", max_sample=" + lexical_cast<string>(max_sample) + ") " + fname + extension;
  
  //Variables
  //Loop-Counter
  int sample = 0;
  
  //For using SOM
  dmat x = data::generator(fpath, 4);
  dmat t = data::generator(fpath, 5, 7);
  std::vector<dlist> errVector;
  ofstream f(fname_err.c_str(), ios::out);

  perceptron mip(&x, t, 3);

  while(sample < max_sample){
    cout << sample << " / " << max_sample << endl;

    errVector.push_back(mip.som_learning(learning_time, som_learning_time, dimension, p_eta, s_eta));

    ++sample;
  }

  //Compute MIP's average error
  for(int i = 0; i < learning_time; ++i){
    double cumulo_error = 0.0;
    std::vector<dlist>::iterator it = errVector.begin();
    for(int j = 0; j < max_sample; ++j, ++it){
      cumulo_error += *it->begin();
      it->pop_front();
    }
    f << cumulo_error / max_sample << std::endl;
  }

  return 0;
}


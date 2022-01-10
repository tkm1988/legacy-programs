#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include "mlp.h"
#include "data.h"

using namespace std;
using namespace boost;
using namespace boost::numeric::ublas;

typedef list<double> dlist;
typedef matrix<double> dmat;
typedef boost::numeric::ublas::vector<double> dvect;

int main(int, char **);

int main(int argc, char *argv[]){
  int sample = 0;
  int oneuron_num = 3;
  int const max_sample = 100;
  int const hlayer_num = 1;
  int const hneuron_num = 5;
  int const lt = 500;
  double const eta = 0.1;
  string const fname_err = "error/mlp_error(iris_lt=" + lexical_cast<string>(lt) + "_hn=" + lexical_cast<string>(hneuron_num) +").dat";
  ofstream f(fname_err.c_str());
  std::vector<dlist> ErrListVect;
  dmat x = data::generator("dataset/iris.dat", 4);
  dmat t = data::generator("dataset/iris.dat", 4, 6);

  while(sample < max_sample){
    mlp p(&x, &t, hlayer_num, hneuron_num, oneuron_num);
    
    try{
      ErrListVect.push_back(p.learning(lt, eta));
    }catch(char *e){
      cout << "Exception [p.learning(int, double)]" << e << endl;
    }

    ++sample;
  }
  
  for(int i = 0; i < lt; ++i){
    double CumuloError = 0.0;
    for(std::vector<dlist>::iterator it = ErrListVect.begin(); it != ErrListVect.end(); ++it){
      CumuloError += *it->begin();
      it->pop_front();
    }
    f << CumuloError / (double)max_sample << endl;
  }

  return 0;
}

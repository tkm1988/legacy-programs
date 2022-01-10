#include <cmath>
#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include "data.h"
#include "som.hpp"
//#include "eval_org.hpp"

using namespace std;
using namespace boost;
using namespace boost::numeric::ublas;

typedef matrix<double> dmat;
typedef matrix_row<dmat> dmat_r;
typedef list<double> dlist;

int main(){
  //Constant
  int const learning_time = 50;
  int const dimension = 2;
  int length = 0;
  double const br[3] = {10, 50, 90};
  double const eta = 0.1;
  string const str_template = "bncoord(iris, br";
  string fname = "";
  ofstream ofs;
  std::vector<int> broken_neuron_index_vect;

  //Learning data & Teacher data
  dmat x = data::generator("dataset/iris.dat", 4);
  dmat t = data::generator("dataset/iris.dat", 5, 7);
  //dmat x = data::generator("dataset/segmentation.dat", 4, 19);
  //dmat t = data::generator("dataset/segmentation.dat", 3);
  //dmat x = data::generator("dataset/movement_libras.dat", 90);
  //dmat t = data::generator("dataset/movement_libras.dat", 91, 94);
  //dmat x = data::generator("dataset/winequality-red.dat", 11);
  //dmat t = data::generator("dataset/winequality-red.dat", 12, 15);

  length = (int)sqrt(x.size1() * 0.9);

  for(int i = 0; i < 3; ++i){
    //Self-Organization Map
    som s(length, length);
    dlist Error;
    
    //Learn
    try{
      cout << "Learning" << endl;
      Error = s.Learning(&x, learning_time, dimension, eta);
      cout << "Broken" << endl;
      s.broken_neuron(br[i]);
      broken_neuron_index_vect = s.getBNIndexList();
      cout << "End" << endl;
    }catch(char *e){
      cout << e << endl;
    }

    fname = str_template + lexical_cast<string>(br[i]) + ").dat";
    cout << fname << endl;
    ofs.open(fname.c_str());
    for(std::vector<int>::iterator it =  broken_neuron_index_vect.begin(), eit = broken_neuron_index_vect.end(); it != eit; ++it){
      ofs << *it - floor(static_cast<double>(*it) / static_cast<double>(length)) * length << " " << floor(static_cast<double>(*it) / static_cast<double>(length)) << endl;
    }
    ofs.close();
    broken_neuron_index_vect.clear();
  }

  return 0;
}

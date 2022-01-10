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
#include "eval_org.hpp"

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
  double const eta = 0.1;


  //Learning data & Teacher data
  //dmat x = data::generator("dataset/iris.dat", 4);
  //dmat t = data::generator("dataset/iris.dat", 5, 7);
  //dmat x = data::generator("dataset/segmentation.dat", 4, 19);
  //dmat t = data::generator("dataset/segmentation.dat", 3);
  //dmat x = data::generator("dataset/movement_libras.dat", 90);
  //dmat t = data::generator("dataset/movement_libras.dat", 91, 94);
  dmat x = data::generator("dataset/winequality-red.dat", 11);
  dmat t = data::generator("dataset/winequality-red.dat", 12, 15);
  EvalOrg eo;
  double result = 0.0;
  double br [3] = {10, 50, 90};

  eo.setInput(&x);
  eo.setTeacher(&t);
  //eo.setRepPtNum(15);
  //eo.setRepPtNum(21);
  //eo.setRepPtNum(36);
  eo.setRepPtNum(160);
  eo.setNearPtNum(5);

  for(int i = 0; i < 3; ++i){
    //Self-Organization Map
    som s((int)sqrt((x.size1() * 0.9)), (int)sqrt((x.size1() * 0.9)));
    dlist Error;
    
    //Learn
    try{
      Error = s.Learning(&x, learning_time, dimension, eta);
      //s.write_map(&t, learning_time, "feature/som/iris/");
      //s.write_map(&t, learning_time, "feature/som/segmentation/");
      //s.write_map(&t, learning_time, "feature/som/movement_libras/");
      s.write_map(&t, learning_time, "feature/som/winequality-red/");
      s.broken_neuron(br[i]);
      //s.write_map(&t, learning_time, "feature/som/iris/broken/" + lexical_cast<string>(br[i]) + "/");
      s.write_map(&t, learning_time, "feature/som/winequality-red/broken/" + lexical_cast<string>(br[i]) + "/");
      result = eo.eval(&s, "k-means++");
    }catch(char *e){
      cout << e << endl;
    }
    
    cout << result << endl;
    data::write_list("error/som/error.dat", Error);
    data::write_mat("error/som/weight.dat", s.getRefVector());
  }
  return 0;
}

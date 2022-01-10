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
  int learning_time [4]= {0, 5, 10, 50};
  int const dimension = 2;
  double const eta = 0.1;
  double result = 0.0;

  //Learning data & Teacher data
  dmat x = data::generator("dataset/movement_libras.dat", 90);
  dmat t = data::generator("dataset/movement_libras.dat", 91, 94);
  EvalOrg eo;
  eo.setInput(&x);
  eo.setTeacher(&t);
  eo.setRepPtNum(36);
  eo.setNearPtNum(5);

  for(int i = 4; i < 4; ++i){
    //Self-Organization Map
    som s((int)sqrt((x.size1() * 0.9)), (int)sqrt((x.size1() * 0.9)));
    dlist Error;

    //Learn
    try{
      Error = s.Learning(&x, learning_time[i], dimension, eta);
      s.write_map(&t, learning_time[i], "feature/som/movement_libras/");
      result = eo.eval(&s, "k-means++");
    }catch(char *e){
      cout << e << endl;
    }
    
    cout << learning_time[i] << "\t" << result << endl;
    data::write_list("error/som/error.dat", Error);
    data::write_mat("error/som/weight.dat", s.getRefVector());
  }

  return 0;
}

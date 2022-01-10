#include "../data.h"
#include "GUI_SOM/som.hpp"
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp> 
#include <boost/numeric/ublas/vector.hpp> 
#include <boost/numeric/ublas/matrix_proxy.hpp>

using namespace std;
using namespace boost;
using namespace boost::numeric::ublas;

typedef matrix<double>				dmat;
typedef boost::numeric::ublas::vector<double>	dvect;

int main(void);

int main(){
  dmat x = data::generator("GUI_SOM/point.txt", 2);

  som s = som(15, 15, 2);
  s.setData(&x);
  for(int i = 0; i < 300; ++i){
    //s.ProdLearning(300, 0.9, 7.5, true);
    s.NormLearning(300, 0.9, 5.0, true);
  }
  for(int i = 0;i < 100; ++i){
    cout << s.getRefMatrix()(i, 0) << "\t" << s.getRefMatrix()(i, 1) << endl;
  }
  return 0;
}

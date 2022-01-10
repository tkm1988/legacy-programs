#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <string>
#include <boost/random.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>

using namespace std;
using namespace boost;
using namespace boost::numeric::ublas;

typedef matrix<double> dmat;

int main(int, char **);
dmat preKalmanState(int, dmat* , dmat* , dmat*);
dmat preKalmanCov(int, dmat*, dmat* , dmat*);
double det(dmat*);
dmat inv(dmat*);

mt19937 gen(static_cast<unsigned long>(time(0)));
normal_distribution<> dst(0, 1);
variate_generator<mt19937&, normal_distribution<> > rnd(gen, dst);

int main(int argc, char **argv){
  dmat A(2,2);
  dmat G(2,1);
  dmat Sigma;
  dmat initState(2,1);
  dmat initCov(2,2);
  dmat preState;
 
  A(0,0) = 1;A(0,1) = 1;
  A(1,0) = 0;A(1,1) = 1;

  G(0,0) = 0.5;
  G(1,0) = 1;
  
  initState(0,0) = 0;
  initState(1,0) = 0;

  initCov(0,0) = 0;initCov(0,1) = 0;
  initCov(1,0) = 0;initCov(1,1) = 0;

  preState = preKalmanState(strtol(argv[1],NULL,10), &initState, &A, &G);
  Sigma = preKalmanCov(strtol(argv[1],NULL,10), &initCov, &A, &G);

  for(int point = -20; point <= 20; ++point){
    for(int verocity = -10; verocity <= 10; ++verocity){
      dmat nowState(2,1);
      dmat tmp1 = 2 * M_PI * Sigma;
      dmat tmp3 = prod(trans(nowState - preState), inv(&Sigma));
      double tmp2 = (-0.5 * prod(tmp3, nowState - preState))(0,0);
      double prob = 0.0;
      nowState(0,0) = point;
      nowState(1,0) = verocity;
      
      prob = det(&tmp1) * exp(tmp2);

      cout << point << "\t" << verocity << "\t" << prob << endl;
    }
  }

  return 0;
}

dmat preKalmanState(int k, dmat* State, dmat* A, dmat* G){
  dmat tmp = prod(*A, *State) + (*G)*rnd();

  if(k > 1){
    return preKalmanState(--k, &tmp, A, G);
  }else{
    //cout << tmp << endl;
    return tmp;
  }
}

dmat preKalmanCov(int k, dmat* Cov, dmat* A, dmat* G){
  dmat tmp = prod(*A, *Cov);
  dmat tmp1 = prod(tmp, trans(*A)) + prod(*G, trans(*G));
  if(k > 1){
    return preKalmanCov(--k, &tmp1, A, G);
  }else{
    //cout << tmp1 << endl;
    return tmp1;
  }
}

double det(dmat* matrix){
  if(static_cast<int>(matrix->size1()) == 2 && static_cast<int>(matrix->size2()) == 2){
    return ((*matrix)(0,0) * (*matrix)(1,1) - (*matrix)(0,1) * (*matrix)(1,0));
  }else{
    return 0.0;
  }
}

dmat inv(dmat* matrix){
  if(static_cast<int>(matrix->size1()) == 2 && static_cast<int>(matrix->size2()) == 2){
    dmat tmp(2,2);
    tmp(0,0) = (*matrix)(1,1);tmp(0,1) = -(*matrix)(0,1);
    tmp(1,0) = -(*matrix)(1,0);tmp(1,1) = (*matrix)(0,0);

    return (1.0 / abs(det(matrix)) * tmp);
  }else{
    return *matrix;
  }
}

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include "KalmanFilter.hpp"

int main(void);

int main (){
  int const		accSigma    = 1;
  int const		mesureSigma = 10;
  double const		R	    = 10.0;
  double const		deltaT	    = 1.0;
  double const		Mesure	    = 5.0;
  dmat			A(2,2), B(2,2), G(2,1), Q(2,2), H(1,2);
  dmat			State(2,1), Cov(2,2), Ctrl(2,1);
  std::vector<dmat>	Result;

  A(0,0) = 1;A(0,1) = deltaT;
  A(1,0) = 0;A(1,1) = 1;

  B(0,0) = 0;B(0,1) = 0;
  B(1,0) = 0;B(1,1) = 0;

  G(0,0) = deltaT * deltaT * 0.5;
  G(1,0) = deltaT;
  Q = accSigma * prod(G, trans(G));
  
  H(0,0) = 1;H(0,1) = 0;

  State(0,0) = 0;
  State(1,0) = 0;

  Cov(0,0) = 1;Cov(0,1) = 0;
  Cov(1,0) = 0;Cov(1,1) = 1;

  Ctrl(0,0) = 0;
  Ctrl(1,0) = 0;

  KalmanFilter kf = KalmanFilter(&A, &B, &Q, &H, R, accSigma, mesureSigma);

  cout << "Initial State : " << State << "\nInitial Cov : " << Cov << endl;
  
  for(int i = 0; i < 5; ++i){
    if(i < 4){
      Result = kf.Filtering(&State, &Cov, &Ctrl, &G);
    }else{
      Result = kf.Filtering(&State, &Cov, &Ctrl, &G, Mesure);
    }

    State = Result.at(0);
    Cov = Result.at(1);
    cout << "Posteriori State : " << State << "\nPosteriori Cov : " << Cov << endl;
  }

  return 0;
}

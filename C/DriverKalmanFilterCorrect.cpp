#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include "KalmanFilter.hpp"

int main(int, char**);

int main (int argc, char **argv){
  int const		accSigma    = 1;
  int const             t = (argc == 2)?(lexical_cast<int>(argv[1])):(4);
  double const		deltaT	    = 1.0;
  dmat			A(2,2), B(2,2), G(2,1), Q(2,2);
  dmat			State(2,1), Cov(2,2), Ctrl(2,1);
  std::vector<dmat>	Result;

  A(0,0) = 1;A(0,1) = deltaT;
  A(1,0) = 0;A(1,1) = 1;

  B(0,0) = 0;B(0,1) = 0;
  B(1,0) = 0;B(1,1) = 0;

  G(0,0) = deltaT * deltaT * 0.5;
  G(1,0) = deltaT;
  Q = accSigma * prod(G, trans(G));
  
  State(0,0) = 0;
  State(1,0) = 0;

  Cov(0,0) = 1;Cov(0,1) = 0;
  Cov(1,0) = 0;Cov(1,1) = 1;

  Ctrl(0,0) = 0;
  Ctrl(1,0) = 0;

  KalmanFilter kf = KalmanFilter(&A, &B, &Q, accSigma);

  for(int i = 0; i < t; ++i){
    string fileName = "KFCorrect(k=" + lexical_cast<string>(i + 1) + ").txt";
    ofstream ofs(fileName.c_str());
    Result = kf.Correct(&State, &Cov, &Ctrl, &G);
    State = Result.at(0);
    Cov = Result.at(1);

    for(double location = -20.0; location <= 20.0; location += 0.5){
      for(double vel = -10.0; vel <= 10.0; vel += 0.5){
	dmat Info(2,1);
	Info(0,0) = location;
	Info(1,0) = vel;
	double prob = kf.calcBelief(&Info, &State, &Cov);
	
	ofs << location << "\t" << vel << "\t" << prob << endl;
      }
    }
  }

  return 0;
}

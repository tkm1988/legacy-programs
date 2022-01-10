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
  dmat preState(2,1), preCov(2,2), postState(2,1), postCov(2,2);

  preState(0,0) = 25.3376;
  preState(1,0) = 8.8041;

  preCov(0,0) = 5.44172;preCov(0,1) = 2.17169;
  preCov(1,0) = 2.17169;preCov(1,1) = 1.9313;

  postState(0,0) = 18.5045;
  postState(1,0) = 3.63359;

  postCov(0,0) = 5.44759;postCov(0,1) = 2.09547;
  postCov(1,0) = 2.09547;postCov(1,1) = 1.96676;

  string fileName1 = "KF(t=4).txt";
  string fileName2 = "KF(t=5).txt";
  ofstream ofs1(fileName1.c_str());
  ofstream ofs2(fileName2.c_str());

  for(double location = 0.0; location <= 40.0; location += 0.5){
    for(double vel = 0.0; vel <= 20.0; vel += 0.5){
      dmat Info(2,1);
      Info(0,0) = location;
      Info(1,0) = vel;
      double prob = KalmanFilter::calcBelief(&Info, &preState, &preCov);
      
      ofs1 << location << "\t" << vel << "\t" << prob << endl;
    }
  }

  for(double location = 0.0; location <= 40.0; location += 0.5){
    for(double vel = 0.0; vel <= 20.0; vel += 0.5){
      dmat Info(2,1);
      Info(0,0) = location;
      Info(1,0) = vel;
      double prob = KalmanFilter::calcBelief(&Info, &postState, &postCov);
      
      ofs2 << location << "\t" << vel << "\t" << prob << endl;
    }
  }
  

  return 0;
}

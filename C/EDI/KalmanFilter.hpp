#pragma once

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <boost/random.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include "ublasdeterminant.hpp"
#include "ublasinvert.hpp"

using namespace std;
using namespace boost;
using namespace boost::numeric::ublas;

typedef matrix<double> dmat;

class KalmanFilter{
private:
  int accSigma, mesureSigma;
  double R;
  dmat *A,*B,*Q,*H;
  dmat KalmanGain;
public:
  KalmanFilter(dmat*,dmat*,dmat*, int const);
  KalmanFilter(dmat*,dmat*,dmat*,dmat*, double const, int const, int const);
  ~KalmanFilter();
  std::vector<dmat> Filtering(dmat*, dmat*, dmat*, dmat*, double const);
  std::vector<dmat> Filtering(dmat*, dmat*, dmat*, dmat*);
  std::vector<dmat> Correct(dmat*, dmat*, dmat*, dmat*);
  std::vector<dmat> Predict(dmat*, dmat*, double const);
  static double calcBelief(dmat*, dmat*, dmat*);
};

KalmanFilter::KalmanFilter(dmat* A, dmat* B, dmat* Q, int const accSigma){
  this->A	   = A;
  this->B	   = B;
  this->Q	   = Q;
  this->accSigma    = accSigma;
}

KalmanFilter::KalmanFilter(dmat* A, dmat* B, dmat* Q, dmat* H, double const R, int const accSigma, int const mesureSigma){
  this->A	   = A;
  this->B	   = B;
  this->Q	   = Q;
  this->H	   = H;
  this->R	   = R;
  this->accSigma    = accSigma;
  this->mesureSigma = mesureSigma;
}

KalmanFilter::~KalmanFilter(){
}

std::vector<dmat> KalmanFilter::Filtering(dmat* State, dmat* Cov, dmat* Ctrl, dmat* G, double const Mesure){
  std::vector<dmat> CorrectResult = KalmanFilter::Correct(State, Cov, Ctrl, G);
  std::vector<dmat> PredictResult = KalmanFilter::Predict(&(CorrectResult.at(0)), &(CorrectResult.at(1)), Mesure);

  return PredictResult;
}

std::vector<dmat> KalmanFilter::Filtering(dmat* State, dmat* Cov, dmat* Ctrl, dmat* G){
  mt19937			mesureGen(static_cast<unsigned long>(time(0)));
  normal_distribution<> mesureDst(0, mesureSigma);
  variate_generator<mt19937&, normal_distribution<> > GaussNoise(mesureGen, mesureDst);
  double                const	Mesure = (prod(*H, *State))(0,0) + GaussNoise();

  return KalmanFilter::Filtering(State, Cov, Ctrl, G, Mesure);
}

std::vector<dmat> KalmanFilter::Correct(dmat* State, dmat* Cov, dmat* Ctrl, dmat* G){
  dmat			prioriState(State->size1(), State->size2());
  dmat			prioriCov(Cov->size1(), Cov->size2());
  std::vector<dmat>	Result;
  mt19937		accGen(static_cast<unsigned long>(time(0))); 
  normal_distribution<> accDst(0, accSigma);
  variate_generator<mt19937&, normal_distribution<> > Acceleration(accGen, accDst);

  prioriState = prod(*A, *State) + prod(*B, *Ctrl) + (*G) * Acceleration();
  prioriCov   = prod(*A, *Cov);
  prioriCov   = prod(prioriCov, trans(*A)) + *Q;
  
  Result.push_back(prioriState);
  Result.push_back(prioriCov);

  return Result;  
}

std::vector<dmat> KalmanFilter::Predict(dmat* prioriState, dmat* prioriCov, double const Mesure){
  dmat			posterioriState(prioriState->size1(), prioriState->size2());
  dmat			posterioriCov(prioriCov->size1(), prioriCov->size2());
  std::vector<dmat>	Result;
  
  KalmanGain = prod(*H, *prioriCov);
  KalmanGain = prod(*prioriCov, trans(*H)) / (prod(KalmanGain, trans(*H))(0,0) + R);
  posterioriState = *prioriState + KalmanGain * (Mesure - (prod(*H, *prioriState))(0,0));
  posterioriCov = identity_matrix<double>(posterioriCov.size1()) - prod(KalmanGain, *H);
  posterioriCov = prod(posterioriCov, *prioriCov);

  Result.push_back(posterioriState);
  Result.push_back(posterioriCov);

  return Result;
}

double KalmanFilter::calcBelief(dmat* Info, dmat* State, dmat* Cov){
  dmat invertCov;
  ublasinvert::invert(*Cov, invertCov);
  dmat Gap = prod(trans(*Info - *State), invertCov);
  return 1 / sqrt(ublasdeterminant::determinant(2.0 * M_PI * (*Cov))) * exp(-0.5 * prod(Gap, (*Info - *State))(0,0));
}

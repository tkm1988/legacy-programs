#include "MyThread.hpp"
#include "som.hpp"

MyThread::MyThread(QObject *parent, int *x, int *y, int *init_ltime, double * init_alpha, double *init_sigma,
		   bool *ModeFlag, bool *TypeFlag, double *current_alpha, double *current_sigma, int *current_ltime,
		   dmat *Data, dmat *RefMatrix):QThread(parent){
  MyThread::x		  = x;
  MyThread::y		  = y;
  MyThread::init_ltime	  = init_ltime;
  MyThread::init_alpha	  = init_alpha;
  MyThread::init_sigma	  = init_sigma;
  MyThread::ModeFlag	  = ModeFlag;
  MyThread::TypeFlag	  = TypeFlag;
  MyThread::current_alpha = current_alpha;
  MyThread::current_sigma = current_sigma;
  MyThread::current_ltime = current_ltime;
  MyThread::Data	  = Data;
  MyThread::RefMatrix     = RefMatrix;
}

void MyThread::run() {
  if(*TypeFlag == true){
    s = new som(*x, *y, static_cast<int>(Data->size2()));
    s->setData(Data);
    s->setLtimeLimit(*init_ltime);
    for(int clt_max = *init_ltime; *current_ltime < clt_max; ++(*current_ltime)){
      if(*ModeFlag == true){
	s->ProdLearning(*init_ltime, *init_alpha, *init_sigma, true);
      }else{
	s->NormLearning(*init_ltime, *init_alpha, *init_sigma, true);
      }

      *current_alpha = s->getAlpha();
      *current_sigma = s->getSigma();
      *RefMatrix = s->getRefMatrix();
    }
  }else{
    bs = new BatchSOM(*x, *y, Data);
    bs->setLtimeLimit(*init_ltime);
    for(int clt_max = *init_ltime; *current_ltime < clt_max; ++(*current_ltime)){
      if(*ModeFlag == true){
	bs->ProdLearning(*init_ltime, true);
      }else{
	bs->NormLearning(*init_ltime, true);
      }

      *RefMatrix = bs->getRefMatrix();
    }
  }
  exec();
}

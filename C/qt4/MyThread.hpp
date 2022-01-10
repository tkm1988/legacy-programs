#pragma once

#include <QThread>
#include <boost/numeric/ublas/matrix.hpp>

typedef boost::numeric::ublas::matrix<double> dmat;

class som;
class BatchSOM;

class MyThread : public QThread{
  Q_OBJECT

  public:
  MyThread(QObject *parent, int *, int *, int *, double *, double *, bool *, bool *, double *, double *, int *, dmat *, dmat *);

private:
  som *s;
  BatchSOM *bs;
  int *x;
  int *y;
  int *init_ltime;
  double *init_alpha;
  double *init_sigma;
  bool *ModeFlag;
  bool *TypeFlag;
  double *current_alpha;
  double *current_sigma;
  int *current_ltime;
  dmat *Data;
  dmat *RefMatrix;
protected:
  void run();

};

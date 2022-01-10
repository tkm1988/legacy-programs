//[Header]====================================================================
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include "data.h"
#include "som.hpp"
#include "mip.hpp"
#include "eval_org.hpp"

using namespace std;
using namespace boost;
using namespace boost::numeric::ublas;

typedef matrix<double> dmat;
typedef boost::numeric::ublas::vector<double> bst_dvect;
typedef list<double> dlist;

int main(int, char **);
int main(int argc, char *argv[]){
  int k = 0;
  int sample = 0;
  int per_lt = 0;
  int som_lt = 0;
  int const np = 5;
  int const max_sample = 100;
  int const dim = 2;
  double sum_match = 0;
  double const per_eta = 0.1;
  double const som_eta = 0.1;
  string mode = "";
  string fname = "";
  string fname_vq_err = "";
  string fname_learn_err = "";
  string fpath = "/Users/tkm/programming/c/dataset/";
  string const fpath_err = "/Users/tkm/programming/c/error/broken_evalorg3/";
  ofstream fvq;
  ofstream fl;
  dmat x;
  dmat t;
  std::vector<dlist> VQErrListVect;
  std::vector<dlist> LErrListVect;
  EvalOrg eo;
  mip m;

  if(argc > 1 && argc <= 6){
    k = (int)strtol(argv[1], NULL, 10);
    per_lt = (int)strtol(argv[2], NULL, 10);
    som_lt = (int)strtol(argv[3], NULL, 10);

    if(k <= 0){
      cout << "Error : Number of represent point is bigger than 0." << endl;
      exit(-1);
    }
    if(per_lt < 0){
      cout << "Error : Learning time is bigger than 0." << endl;
      exit(-1);
    }
    if(som_lt < 0){
      cout << "Error : Learning time is bigger than 0." << endl;
      exit(-1);
    }
    if(strcmp(argv[4], "abalone") == 0 ||
       strcmp(argv[4], "iris") == 0 ||
       strcmp(argv[4], "movement_libras") == 0 ||
       strcmp(argv[4], "segmentation") == 0 ||
       strcmp(argv[4], "winequality-red") == 0){
      fname = argv[4];
      fpath += fname + ".dat";
      if(fname == "abalone"){
	x = data::generator(fpath, 4, 11);
	t = data::generator(fpath, 3);
      }else if(fname == "iris"){
	x = data::generator(fpath, 4);
	t = data::generator(fpath, 5, 7);	
      }else if(fname == "movement_libras"){
	x = data::generator(fpath, 90);
	t = data::generator(fpath, 91, 94);
      }else if(fname == "segmentation"){
	x = data::generator(fpath, 4, 19);
	t = data::generator(fpath, 3);
      }else if(fname == "winequality-red"){
	x = data::generator(fpath, 11);
	t = data::generator(fpath, 12, 15);
      }
    }else{
      cout << "Error : A dataset file that you fed isn't registered by programmer." << endl;
      exit(-1);
    }
    if(argc == 5){
      if(strcmp(argv[5], "pp") == 0){
	mode = "k-means++";
      }else{
	mode = "k-means";
	cout << "!CAUTION!\n RepPt method was forcibly set up \"k-means\".";
      }
    }else{
      mode = "k-means";
    }
  }else{
    cout << "Please feed this example." << endl;
    cout << "\t\"/broken_evalorg3 [Num of RepPt] [leaening time(Per)] [leaening time(SOM)] [dataset's filename(DO NOT INCLUDE EXTENSION)] [none or pp]\"" << endl;
    exit(-1);
  }

  eo.setInput(&x);
  eo.setTeacher(&t);
  eo.setRepPtNum(k);
  eo.setNearPtNum(np);

  fname_vq_err = fpath_err + fname + "_vq_error(k=" + lexical_cast<string>(k) + ", lt=" + lexical_cast<string>(som_lt) + ").dat";
  fname_learn_err = fpath_err + fname + "_learn_error(k=" + lexical_cast<string>(k) + ", som_lt=" + lexical_cast<string>(som_lt) + ", per_lt=" + lexical_cast<string>(per_lt) + ").dat";
  fvq.open(fname_vq_err.c_str());
  fl.open(fname_learn_err.c_str());

  VQErrListVect.clear();
  LErrListVect.clear();

  while(sample < max_sample){
    cout << sample + 1 << endl;
    som s((int)sqrt((x.size1() * 0.9)), (int)sqrt((x.size1() * 0.9)));
    
    try{
      VQErrListVect.push_back(s.Learning(&x, som_lt, dim, som_eta));
      m.setSOM(&s);
      LErrListVect.push_back(m.learning(&x, &t, per_lt, per_eta));
      sum_match += eo.broken_eval(&s, mode);
    }catch(char *e){
      cout << "!Exception [evalorg] L134-137 : " << e << endl;
    }
    ++sample;
  }

  cout << "Result:" << endl;
  cout << "\t" << (sum_match / (double)max_sample) << endl;

  for(int i = 0; i < som_lt; ++i){
    double cumulo_error = 0.0;
    std::vector<dlist>::iterator it = VQErrListVect.begin();
    for(int j = 0; j < max_sample; ++j, ++it){
      cumulo_error += *it->begin();
      it->pop_front();
    }
    fvq << (cumulo_error / (double)max_sample) << std::endl;
  }

  for(int i = 0; i < per_lt; ++i){
    double cumulo_error = 0.0;
    std::vector<dlist>::iterator it = LErrListVect.begin();
    for(int j = 0; j < max_sample; ++j, ++it){
      cumulo_error += *it->begin();
      it->pop_front();
    }
    fl << (cumulo_error / (double)max_sample) << std::endl;
  }

  return 0;
}

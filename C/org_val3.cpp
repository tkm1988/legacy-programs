//[Header]====================================================================
#include <cstdlib>
#include <cfloat>
#include <cmath>
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
#include "rnd_shuffle.h"
#include "k-means.hpp"
#include "perceptron.h"

using namespace std;
using namespace boost;
using namespace boost::numeric::ublas;

typedef matrix<double> dmat;
typedef boost::numeric::ublas::vector<double> bst_dvect;
typedef list<double> dlist;
typedef struct{
  bst_dvect val;
  double dist;
}REP_DATA;
typedef struct{
  double dist;
  int point1;
  int point2;
}DDATA;

//Predicator
class BDVequal{
private:
  bst_dvect member;
public:
  BDVequal(bst_dvect foo){
    member = foo;
  }

  bool operator()(bst_dvect bar){
    bool eq = false;
    
    if(member.size() == bar.size()){
      for(int i = 0; i < (int)member.size(); ++i){
	if(member(i) == bar(i)){
	  eq = true;
	}else{
	  eq = false;
	  break;
	}
      }
    }
    return eq;
  }
};

//prototype declaration
int main(int, char**);
double factorial(int);
double combination(int, int);
//bool LessDist(const DDATA&, const DDATA&);
bool descending(const REP_DATA&, const REP_DATA&);
bool ascending(const REP_DATA&, const REP_DATA&);

int main(int argc, char** argv){
  int k = 0;
  int sample = 0;
  int lt = 0;
  int count_cmpt = 0;
  int const nn = 5;
  int const max_sample = 100;
  int const dim = 2;
  double match = 0;
  double const eta = 0.1;
  string mode = "";
  string fname = "";
  string fname_err = "";
  string fname_map = "";
  string fpath = "/Users/tkm/programming/c/dataset/";
  string const fpath_err = "/Users/tkm/programming/c/error/";
  ofstream f;
  dmat x;
  dmat t;
  std::vector<dlist> ErrListVect;
  std::vector<bst_dvect> Dataset_RepPt_Vect;
  std::vector<bst_dvect> Som_RepPt_Vect;
  //DON'T USE THESE VARIABLES (Use Wrong Method)
  //std::vector<DDATA> Dataset_DDATA_Vect;
  //std::vector<DDATA> Som_DDATA_Vect;

  if(argc > 1 && argc <= 5){
    k = (int)strtol(argv[1], NULL, 10);
    lt = (int)strtol(argv[2], NULL, 10);
    
    if(k <= 0){
      cout << "Error : Number of represent point is bigger than 0." << endl;
      exit(-1);
    }
    if(lt < 0){
      cout << "Error : Learning time is bigger than 0." << endl;
      exit(-1);
    }
    if(strcmp(argv[3], "abalone") == 0 ||
       strcmp(argv[3], "iris") == 0 ||
       strcmp(argv[3], "movement_libras") == 0 ||
       strcmp(argv[3], "segmentation") == 0 ||
       strcmp(argv[3], "winequality-red") == 0){
      fname = argv[3];
      fpath += fname + ".dat";
      if(fname == "abalone"){
	x = data::generator(fpath, 4, 11);
	t = data::generator(fpath, 3);
      }else if(fname == "iris"){
	x = data::generator(fpath, 4);
	t = data::generator(fpath, 4, 6);	
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
      if(strcmp(argv[4], "pp") == 0){
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
    cout << "\t\"/org_val3 [number of representation point (integer)] [leaening time(integer)] [dataset's filename(DO NOT INCLUDE EXTENSION)] [none or pp]\"" << endl;
    exit(-1);
  }
  
  fname_err = fpath_err + fname + "_error(lt=" + lexical_cast<string>(lt) + "_k=" + lexical_cast<string>(k) + ").dat";
  fname_map = fname + "_feature(lt=" + lexical_cast<string>(lt) + "_k=" + lexical_cast<string>(k) + ")";
  f.open(fname_err.c_str());

  ErrListVect.clear();
  while(sample < max_sample){
    ++sample;
    cout << sample << " / " << max_sample << endl;

    bool do_continue = false;
    som s = som();
    ErrListVect.push_back(s.Learning(&x, lt, dim, eta));

    //rnd_shuffle rs((int)x.size1());
    //std::vector<int> RandShuffleVect = rs.getshuffled();
    //std::vector<int>::iterator RSVit = RandShuffleVect.begin();
    std::vector<REP_DATA> REP_DATA_Vect[k];

    Dataset_RepPt_Vect.clear();
    Som_RepPt_Vect.clear();
    if(mode == "k-means"){
      Dataset_RepPt_Vect = kmeans(k, &x, STANDARD_K_MEANS);
    }else{
      Dataset_RepPt_Vect = kmeans(k, &x, K_MEANS_PLAS_PLAS);
      //Dataset_RepPt_Vect = kmeans(k, &x, K_MEANS_USE_ICA);
    }

    for(int i = 0; i < (int)Dataset_RepPt_Vect.size(); ++i){
      BDVequal::BDVequal eq(s.getCoord(Dataset_RepPt_Vect.at(i)));
      cout << fname << " RepPt : " << Dataset_RepPt_Vect.at(i) << endl;
      cout << "SOM RepPt : " << s.getCoord(Dataset_RepPt_Vect.at(i)) << endl;

      if((int)count_if(Som_RepPt_Vect.begin(), Som_RepPt_Vect.end(), eq) > 0){
	count_cmpt += (nn * k);
	do_continue = true;
	break;
      }

      Som_RepPt_Vect.push_back(s.getCoord(Dataset_RepPt_Vect.at(i)));
    }

    if(do_continue == true){
      continue;
    }

    for(int target = 0; target < (int)x.size1(); ++target){
      int index = -1;
      REP_DATA tmp;
      bst_dvect DNorm2dVect((int)Dataset_RepPt_Vect.size());
      
      for(int i = 0; i < (int)Dataset_RepPt_Vect.size(); ++i){
	DNorm2dVect(i) = norm_2(Dataset_RepPt_Vect.at(i) - row(x, target));
      }
      index = min_element(DNorm2dVect.begin(), DNorm2dVect.end()).index();

      tmp.val = (bst_dvect)row(x, target);
      tmp.dist = DNorm2dVect(index);

      REP_DATA_Vect[index].push_back(tmp);
    }
    
    try{
      for(int i = 0; i < k; ++i){
	stable_sort(REP_DATA_Vect[i].begin(), REP_DATA_Vect[i].end(), descending);

	for(int j = 0; j < nn && j < (int)REP_DATA_Vect[i].size(); ++j, ++count_cmpt){
	  int index = -1;
	  bst_dvect SNorm2dVect((int)Som_RepPt_Vect.size());
	  bst_dvect tmp = s.getCoord(REP_DATA_Vect[i].at(j).val);

	  for(int l = 0; l < (int)Som_RepPt_Vect.size(); ++l){
	    SNorm2dVect(l) = norm_2(Som_RepPt_Vect.at(l) - tmp);
	  }

	  index = min_element(SNorm2dVect.begin(), SNorm2dVect.end()).index();

	  if(index == i){
	    ++match;
	  }else{
	    cout << "Don't Match..." << endl;
	  }
	}
      }
    }catch(char *e){
      cout << "Exception [org_val3] L171 : " << e << endl;
    }

    /*WRONG METHOD
    while(RSVit != RandShuffleVect.end()){
      int dataset_index = -1;
      int som_index = -1;
      bst_dvect DNorm2dVect((int)Dataset_RepPt_Vect.size());
      bst_dvect SNorm2dVect((int)Som_RepPt_Vect.size());
      bst_dvect CodeBookVect = s.getCoord((bst_dvect)row(x, *RSVit));

      for(int i = 0; i < (int)Dataset_RepPt_Vect.size(); ++i){
	DNorm2dVect(i) = norm_2(Dataset_RepPt_Vect.at(i) - row(x, *RSVit));
	SNorm2dVect(i) = norm_2(Som_RepPt_Vect.at(i) - CodeBookVect);
      }
      dataset_index = min_element(DNorm2dVect.begin(), DNorm2dVect.end()).index();
      som_index = min_element(SNorm2dVect.begin(), SNorm2dVect.end()).index();
      
      if(dataset_index == som_index){
	++match;
      }else{
	cout << "WRONG : " << DNorm2dVect << " Min Element's Index : " << dataset_index<< endl;
	cout << "\t" << SNorm2dVect << "\t"<< som_index << endl;
      }
      ++RSVit;
    }
    */

    /*WRONG METHOD
    for(int i = 1; i <= (int)Dataset_RepPt_Vect.size(); ++i){
      DDATA tmp;
      for(int j = i + 1; j <= (int)Dataset_RepPt_Vect.size(); ++j){
	tmp.dist = norm_2(Dataset_RepPt_Vect.at(i - 1) - Dataset_RepPt_Vect.at(j - 1));
	tmp.point1 = i;
	tmp.point2 = j;
	Dataset_DDATA_Vect.push_back(tmp);
	tmp.dist = norm_2(Som_RepPt_Vect.at(i - 1) - Som_RepPt_Vect.at(j - 1));
	Som_DDATA_Vect.push_back(tmp);
      }    
    }
    
    try{
      stable_sort(Dataset_DDATA_Vect.begin(), Dataset_DDATA_Vect.end(), LessDist);
      stable_sort(Som_DDATA_Vect.begin(), Som_DDATA_Vect.end(), LessDist);
    }catch(char *e){
      cout << "Exception [org_val3.cpp] L144-145 : " << e << endl;
    }

    for(int i = 0; i < (int)Dataset_DDATA_Vect.size(); ++i){
      if((Dataset_DDATA_Vect.at(i).point1 == Som_DDATA_Vect.at(i).point1) && (Dataset_DDATA_Vect.at(i).point2 == Som_DDATA_Vect.at(i).point2)){
	match += 1.0;
      }
    }
    */
  }

  cout << "Score : " << endl;
  //cout << "\t" << match << "/" << Dataset_DDATA_Vect.size() << " = " << (match/Dataset_DDATA_Vect.size()) << endl;
  //cout << "\t" << match << "/" << (x.size1() * max_sample) << " = " << (match/(x.size1() * max_sample)) << endl;
  cout << "\t" << match << "/" << count_cmpt << " * " << 100 << " = " << (match/(double)count_cmpt * 100.0) << "[%]"<< endl;

  for(int i = 0; i < lt; ++i){
    double CumuloErr = 0.0;
    std::vector<dlist>::iterator it = ErrListVect.begin();
    for(int j = 0; j < max_sample; ++j, ++it){
      CumuloErr += *it->begin();
      it->pop_front();
    }
    
    f << (CumuloErr / (double)max_sample) << endl;
  }
  
  return 0;
}

//[User-Defined Function]======================================================
//The function "factorial" provide  a function to compute factorial.
double factorial(int num){
  double f = 1;
  for(int i = num; i > 0; --i){
    f *= i;
  }
  return f;
}

//The function "combination" provide a function to compute combination
double combination(int n, int m){
  return  factorial(n)/(factorial(m)*factorial(n - m));
}
/*DON'T USE THIS PREDICATE
bool LessDist(const DDATA& rLeft, const DDATA& rRight){
  return (rLeft.dist < rRight.dist);
}
*/
bool descending(const REP_DATA& rLeft, const REP_DATA& rRight){
  return (rLeft.dist < rRight.dist);
}
bool ascending(const REP_DATA& rLeft, const REP_DATA& rRight){
  return (rLeft.dist > rRight.dist);
}

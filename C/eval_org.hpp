#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include "data.h"
#include "som.hpp"
#include "rnd_shuffle.h"
#include "k-means.hpp"

using namespace std;
using namespace boost;
using namespace boost::numeric::ublas;

typedef matrix<double> dmat;
typedef boost::numeric::ublas::vector<double> bst_dvect;
typedef list<double> dlist;
typedef struct{
  bst_dvect val;
  double dist;
}REP_DATA
;

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

bool descending(const REP_DATA& rLeft, const REP_DATA& rRight){
  return (rLeft.dist < rRight.dist);
}
bool ascending(const REP_DATA& rLeft, const REP_DATA& rRight){
  return (rLeft.dist > rRight.dist);
}

//Evaluate SOM's Organization
class EvalOrg{
private:
  int k;
  int np;
  double match;
  std::vector<bst_dvect> Dataset_RepPt_Vect;
  std::vector<bst_dvect> Som_RepPt_Vect;
  dmat *x;
  dmat *t;
public:
  EvalOrg(void);
  EvalOrg(dmat *, dmat *);
  void setInput(dmat *);
  void setTeacher(dmat *);
  void setRepPtNum(int);
  void setNearPtNum(int);
  std::vector<bst_dvect> getDataSetRepPt();
  std::vector<bst_dvect> getSOMRepPt();
  double eval(som *, string);
  double broken_eval(som *, string);
};
EvalOrg::EvalOrg(){
  k = 0;
  np = 0;
  match = 0.0;
  Dataset_RepPt_Vect.clear();
  Som_RepPt_Vect.clear();
  x = NULL;
  t = NULL;
}
EvalOrg::EvalOrg(dmat *input, dmat *teacher){
  k = 0;
  np = 0;
  match = 0.0;
  Dataset_RepPt_Vect.clear();
  Som_RepPt_Vect.clear();
  x = input;
  t = teacher;
}
void EvalOrg::setInput(dmat *input){
  x = input;
}
void EvalOrg::setTeacher(dmat *teacher){
  t = teacher;
}
void EvalOrg::setRepPtNum(int num){
  k = num;
}
void EvalOrg::setNearPtNum(int num){
  np = num;
}
std::vector<bst_dvect> EvalOrg::getDataSetRepPt(){
  return Dataset_RepPt_Vect; 
}
std::vector<bst_dvect> EvalOrg::getSOMRepPt(){
  return Som_RepPt_Vect; 
}
double EvalOrg::eval(som *s, string mode){
  bool duplication = false;
  int count_cmpt = 0;
  std::vector<REP_DATA> REP_DATA_Vect[k];
  
  match = 0.0;
  Dataset_RepPt_Vect.clear();
  Som_RepPt_Vect.clear();

  if(mode == "k-means"){
    try{
      Dataset_RepPt_Vect = kmeans(k, x, STANDARD_K_MEANS);
    }catch(char *e){
      throw "Exception [eval_org] L126 : " + *e;
    }
  }else if(mode == "k-means++"){
    try{
      Dataset_RepPt_Vect = kmeans(k, x, K_MEANS_PLAS_PLAS);
    }catch(char *e){
      throw "Exception [eval_org] L132 : " + *e;
    }
  }else{
    try{
      Dataset_RepPt_Vect = kmeans(k, x, K_MEANS_USE_ICA);
    }catch(char *e){
      throw "Exception [eval_org] L138 : " + *e;
    }
  }
  
  try{
    for(int i = 0; i < (int)Dataset_RepPt_Vect.size(); ++i){
      BDVequal::BDVequal eq(s->getCoord(Dataset_RepPt_Vect.at(i)));
      if((int)count_if(Som_RepPt_Vect.begin(), Som_RepPt_Vect.end(), eq) > 0){
	count_cmpt += (np * k);
	duplication = true;
	break;
      }
      Som_RepPt_Vect.push_back(s->getCoord(Dataset_RepPt_Vect.at(i)));
    }
  }catch(char *e){
    throw "Exception [EvalOrg] L120 - 131 : " + *e;
  }

  if(duplication){
    return 0.0;
  }

  try{
    for(int target = 0; target < (int)x->size1(); ++target){
      int index = -1;
      REP_DATA tmp;
      bst_dvect DNorm2dVect((int)Dataset_RepPt_Vect.size());
      
      for(int i = 0; i < (int)Dataset_RepPt_Vect.size(); ++i){
	DNorm2dVect(i) = norm_2(Dataset_RepPt_Vect.at(i) - row(*x, target));
      }
      index = min_element(DNorm2dVect.begin(), DNorm2dVect.end()).index();
      
      tmp.val = (bst_dvect)row(*x, target);
      tmp.dist = DNorm2dVect(index);
      
      REP_DATA_Vect[index].push_back(tmp);
    }
  }catch(char *e){
    throw "Exception [EvalOrg] L136 - 150 : " + *e;
  }
  
  try{
    for(int i = 0; i < k; ++i){
      stable_sort(REP_DATA_Vect[i].begin(), REP_DATA_Vect[i].end(), descending);
      
      for(int j = 0; j < np && j < (int)REP_DATA_Vect[i].size(); ++j, ++count_cmpt){
	int index = -1;
	bst_dvect SNorm2dVect((int)Som_RepPt_Vect.size());
	bst_dvect tmp = s->getCoord(REP_DATA_Vect[i].at(j).val);
	
	for(int l = 0; l < (int)Som_RepPt_Vect.size(); ++l){
	  SNorm2dVect(l) = norm_2(Som_RepPt_Vect.at(l) - tmp);
	}
	
	index = min_element(SNorm2dVect.begin(), SNorm2dVect.end()).index();
	
	if(index == i){
	  ++match;
	}else{
	  //cout << "Don't Match..." << endl;
	}
      }
    }
  }catch(char *e){
    throw "Exception [EvalOrg] L171 : " + *e;
  }
  
  return (match/(double)count_cmpt * 100.0);
}
double EvalOrg::broken_eval(som *s, string mode){
  bool duplication = false;
  int count_cmpt = 0;
  std::vector<REP_DATA> REP_DATA_Vect[k];
  
  match = 0.0;
  Dataset_RepPt_Vect.clear();
  Som_RepPt_Vect.clear();

  if(mode == "k-means"){
    try{
      Dataset_RepPt_Vect = kmeans(k, x, STANDARD_K_MEANS);
    }catch(char *e){
      throw "Exception [eval_org] L126 : " + *e;
    }
  }else if(mode == "k-means++"){
    try{
      Dataset_RepPt_Vect = kmeans(k, x, K_MEANS_PLAS_PLAS);
    }catch(char *e){
      throw "Exception [eval_org] L132 : " + *e;
    }
  }else{
    try{
      Dataset_RepPt_Vect = kmeans(k, x, K_MEANS_USE_ICA);
    }catch(char *e){
      throw "Exception [eval_org] L138 : " + *e;
    }
  }
  
  for(std::vector<bst_dvect>::iterator it = Dataset_RepPt_Vect.begin();it != Dataset_RepPt_Vect.end(); ++it){
    s->broken_neuron(*it);
  }

  try{
    for(int i = 0; i < (int)Dataset_RepPt_Vect.size(); ++i){
      BDVequal::BDVequal eq(s->getCoord(Dataset_RepPt_Vect.at(i)));

      if((int)count_if(Som_RepPt_Vect.begin(), Som_RepPt_Vect.end(), eq) > 0){
	count_cmpt += (np * k);
	duplication = true;
	break;
      }
      Som_RepPt_Vect.push_back(s->getCoord(Dataset_RepPt_Vect.at(i)));
    }
  }catch(char *e){
    throw "Exception [EvalOrg] L120 - 131 : " + *e;
  }

  if(duplication){
    return 0.0;
  }
  
  try{
    for(int target = 0; target < (int)x->size1(); ++target){
      int index = -1;
      REP_DATA tmp;
      bst_dvect DNorm2dVect((int)Dataset_RepPt_Vect.size());
      
      for(int i = 0; i < (int)Dataset_RepPt_Vect.size(); ++i){
	DNorm2dVect(i) = norm_2(Dataset_RepPt_Vect.at(i) - row(*x, target));
      }
      index = min_element(DNorm2dVect.begin(), DNorm2dVect.end()).index();
      
      tmp.val = (bst_dvect)row(*x, target);
      tmp.dist = DNorm2dVect(index);
      
      REP_DATA_Vect[index].push_back(tmp);
    }
  }catch(char *e){
    throw "Exception [EvalOrg] L136 - 150 : " + *e;
  }
  
  try{
    for(int i = 0; i < k; ++i){
      stable_sort(REP_DATA_Vect[i].begin(), REP_DATA_Vect[i].end(), descending);
      
      for(int j = 0; j < np && j < (int)REP_DATA_Vect[i].size(); ++j, ++count_cmpt){
	int index = -1;
	bst_dvect SNorm2dVect((int)Som_RepPt_Vect.size());
	bst_dvect tmp = s->getCoord(REP_DATA_Vect[i].at(j).val);
	
	for(int l = 0; l < (int)Som_RepPt_Vect.size(); ++l){
	  SNorm2dVect(l) = norm_2(Som_RepPt_Vect.at(l) - tmp);
	}
	
	index = min_element(SNorm2dVect.begin(), SNorm2dVect.end()).index();
	
	if(index == i){
	  ++match;
	}else{
	  //cout << "Don't Match..." << endl;
	}
      }
    }
  }catch(char *e){
    throw "Exception [EvalOrg] L171 : " + *e;
  }
  
  return (match/(double)count_cmpt * 100.0);
}

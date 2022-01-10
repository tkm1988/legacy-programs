#pragma once

#include <cmath>
#include <cfloat>
#include <ctime>
#include <algorithm>
#include <list>
#include <vector>
#include <map>
#include <boost/lexical_cast.hpp>
#include <boost/random.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp> 
#include <boost/numeric/ublas/vector.hpp> 
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include "rnd_shuffle.h"

using namespace std;
using namespace boost;
using namespace boost::numeric::ublas;

typedef matrix<double> dmat;
typedef matrix_row<matrix<double> > dmat_r;
typedef boost::numeric::ublas::vector<double> dvect;


class som{
private:
  dmat *inputVector;
  dmat *teacher;
  dmat refVector;
  map<string, int> str_hash;
  std::vector<int> broken_neuron_index;
  std::vector<int> broken_neuron_flag;
  int learning_time;
  int xlength;
  int ylength;
  int dim;
  double init_eta;
  double init_sigma;
public:
  som(void);
  som(int const, int const);
  som(dmat *, dmat *, int const, int const, int const);
  std::list<double> Learning(dmat *, int, int, double);
  std::list<double> Learning(int, double);
  std::list<double> Learning_logging(dmat *, dmat *, int, int, double, string);
  int size(void);
  dmat getRefVector();
  dvect getCoord(dmat_r);
  dvect getCoord(dvect);
  void broken_neuron(double const);
  void broken_neuron(dvect const);
  void setTeacher(dmat *);
  void setHash(void);
  void setHash(dmat *);
  map<string, int> getHash(void);
  std::vector<int> getBNIndexList(void);
  bool write_map(string, dmat, som, map<string, int>);
  void write_map(dmat const *, int const, string const);
};

som::som(){
  inputVector = NULL;
  teacher = NULL;
  learning_time = 0;
  xlength = 0;
  ylength = 0;
  dim = 0;
  init_eta = 0.0;
  init_sigma = 0.0;
  broken_neuron_index.clear();
}
som::som(int const xl, int const yl){
  inputVector = NULL;
  teacher = NULL;
  learning_time = 0;
  xlength = xl;
  ylength = yl;
  dim = 0;
  init_eta = 0.0;
  init_sigma = 0.0;
  broken_neuron_index.clear();
  broken_neuron_flag.resize(xlength * ylength);
}
som::som(dmat *inputVector, dmat *t, int const dim, int const xl, int const yl){
  xlength = xl;
  ylength = yl;
  this->inputVector = inputVector;
  this->teacher = t;
  this->dim = dim;
  
  int inputVectorRow =  static_cast<int>(inputVector->size1());
  int inputVectorColumn = static_cast<int>(inputVector->size2());
  mt19937 gen(static_cast<unsigned long>(time(0)));
  uniform_smallint<> dst(1, 10);
  variate_generator<mt19937, uniform_smallint<> > rand(gen, dst);
  broken_neuron_index.clear();
  broken_neuron_flag.resize(xlength * ylength);

  //Define Reference Vector
  if(dim == 1){
    som::xlength = inputVectorRow;	//Reference Vector's xlength == Input Vector's Row
    som::ylength = 1;
    som::refVector.resize(som::xlength, inputVectorColumn);
    
    for(int i = 0; i < som::xlength; ++i){
      for(int j = 0; j < inputVectorColumn; ++j){
	refVector(i, j) = 0.01 * rand();
      }
    }
  }else{
    som::refVector.resize((som::xlength*som::ylength), inputVectorColumn);
    
    for(int i = 0; i < (som::xlength*som::ylength); ++i){
      for(int j = 0; j < inputVectorColumn; ++j){
	refVector(i, j) = 0.01 * rand();
      }
    }
  }
}

list<double> som::Learning(dmat *inputVector, int learning_time, int dim, double init_eta){
  som::inputVector = inputVector;
  som::learning_time = learning_time;
  som::init_eta = init_eta;
  som::dim = dim;
  
  std::list<double> Error;
  double eta = som::init_eta;
  int inputVectorRow =  static_cast<int>(inputVector->size1());
  int inputVectorColumn = static_cast<int>(inputVector->size2());
  
  //*Melsen Twister Generater*****************************************
  mt19937 gen(static_cast<unsigned long>(time(0)));
  uniform_smallint<> dst(1, 10);
  variate_generator<mt19937, uniform_smallint<> > rand(gen, dst);
  
  //*Define Reference Vector******************************************
  //***1D***
  if(dim == 1){
    som::xlength = inputVectorRow;	//ReferenceVector's xlength == Input Vector's Row
    som::ylength = 1;
    som::refVector.resize(som::xlength, inputVectorColumn);
    
    for(int i = 0; i < som::xlength; ++i){
      for(int j = 0; j < inputVectorColumn; ++j){
	refVector(i, j) = 0.01 * rand();
      }
    }
    //***2D***
  }else{
    som::refVector.resize((som::xlength*som::ylength), inputVectorColumn);
    
    //RefereceVectors Initialize by Random Numbers
    for(int i = 0; i < (som::xlength*som::ylength); ++i){
      for(int j = 0; j < inputVectorColumn; ++j){
	refVector(i, j) = 0.01 * rand();
      }
    }
  }
  
  //som::init_sigma = (double)xlength / 2.0;
  som::init_sigma = 20.0;
  double sigma = som::init_sigma;
  double h = 0.0;
  dvect Rc(2), R(2);

  //Learning Part
  for(int t = 0; t < learning_time; ++t){
    double sum_err = 0.0;

    //Renewal sigma's value.
    sigma = init_sigma * pow(0.01 / init_sigma, (double)(t + 1) / learning_time);

    //Make a progression of random numbers.
    rnd_shuffle rs((int)inputVector->size1());
    std::vector<int> rsv = rs.getshuffled();

    //Execute in 1 epoch
    for(std::vector<int>::iterator it = rsv.begin(), it_end = rsv.end(); it != it_end; ++it){
      dvect vnorm((int)refVector.size1());
      
      if(broken_neuron_index.size() == 0){
	for(int i = 0; i < (int)refVector.size1(); ++i){
	  vnorm(i) = norm_2(row(*inputVector, *it) - row(refVector, i));
	}
      }else{
	for(int i = 0; i < (int)refVector.size1(); ++i){
	  if(broken_neuron_flag.at(i) == 0){
	    vnorm(i) = norm_2(row(*inputVector, *it) - row(refVector, i));
	  }else{
	    vnorm(i) = DBL_MAX;
	  }
	}
      }

      int min_index = min_element(vnorm.begin(), vnorm.end()).index();

      Rc(0) = min_index - floor((double)(min_index) / (double)(xlength)) * ylength;
      Rc(1) = floor((double)(min_index) / (double)(xlength));

      for(int y = 0; y < ylength; ++y){
	for(int x = 0; x < xlength; ++x){
	  int const coord = ylength * y + x;
	  R(0) = x;
	  R(1) = y;
	  h = eta * exp(-1.0 * norm_2(Rc - R) * norm_2(Rc - R) / sigma);

	  row(refVector, coord) = row(refVector, coord) + h * (row(*inputVector, *it) - row(refVector, coord));
	}
      }
      sum_err += norm_2(row(*inputVector, *it) - row(refVector, min_index)) * norm_2(row(*inputVector, *it) - row(refVector, min_index));
    }

    Error.push_back(sum_err / (xlength * ylength));
  }
  return Error;
}

list<double> som::Learning(int learning_time, double init_eta){
  som::learning_time = learning_time;
  som::init_eta = init_eta;
  som::init_sigma = 20.0;

  std::list<double> Error;
  double eta = som::init_eta;
  double sigma = som::init_sigma;  
  dvect Rc(2), R(2);
  dvect H(xlength * ylength);

  //Learning Part
  for(int t = 0; t < learning_time; ++t){
    double sum_err = 0.0;

    //Renewal sigma's value.
    sigma = init_sigma * pow(0.01 / init_sigma, (double)(t + 1) / learning_time);

    //Make a progression of random numbers.
    rnd_shuffle rs((int)inputVector->size1());
    std::vector<int> rsv = rs.getshuffled();
    std::vector<int>::iterator it;

    //Execute in 1 epoch
    for(it = rsv.begin(); it != rsv.end(); ++it){
      dvect vnorm((int)refVector.size1());
      
      if(broken_neuron_index.size() == 0){
	for(int i = 0; i < (int)refVector.size1(); ++i){
	  vnorm(i) = norm_2(row(*inputVector, *it) - row(refVector, i));
	}
      }else{
	for(int i = 0; i < (int)refVector.size1(); ++i){
	  if(broken_neuron_flag.at(i) == 0){
	    vnorm(i) = norm_2(row(*inputVector, *it) - row(refVector, i));
	  }else{
	    vnorm(i) = DBL_MAX;
	  }
	}
      }

      int min_index = min_element(vnorm.begin(), vnorm.end()).index();

      Rc(0) = min_index - floor((double)(min_index) / (double)(xlength)) * ylength;
      Rc(1) = floor((double)(min_index) / (double)(xlength));

      for(int y = 0; y < ylength; ++y){
	for(int x = 0; x < xlength; ++x){
	  int const coord = ylength * y + x;
	  R(0) = x;
	  R(1) = y;
	  H(coord) = eta * exp(-1.0 * norm_2(Rc - R) * norm_2(Rc - R) / sigma);

	  row(refVector, coord) = row(refVector, coord) + H(coord) * (row(*inputVector, *it) - row(refVector, coord));
	}
      }
      sum_err += norm_2(row(*inputVector, *it) - row(refVector, min_index)) * norm_2(row(*inputVector, *it) - row(refVector, min_index));
    }
    
    Error.push_back(sum_err / (xlength * ylength));
  }
  return Error;
}

list<double> som::Learning_logging(dmat *inputVector, dmat *teacher, int learning_time, int dim, double init_eta, string directory){
  som::inputVector = inputVector;
  som::teacher = teacher;
  som::learning_time = learning_time;
  som::init_eta = init_eta;
  som::dim = dim;
  som::init_sigma = 20.0;

  std::list<double> Error;
  int msize = xlength * ylength;
  double h = 0.0;
  double eta = som::init_eta;
  double sigma = som::init_sigma;  
  dvect Rc(2), R(2);

  if(teacher == NULL){
    throw "Exception : Please set the value of teacher signal.(boost::numeric::matrix<double>)\n";
  }

  som::write_map(teacher, 0, directory);

  //Learning Part
  cout << "Now, SOM is learning. The learning time is " << learning_time << "." << endl;
  for(int t = 0; t < learning_time; ++t){
    double sum_err = 0.0;

    //Renewal sigma's value.
    sigma = init_sigma * pow(0.01 / init_sigma, (double)(t + 1) / learning_time);

    //Make a progression of random numbers.
    rnd_shuffle rs((int)inputVector->size1());
    std::vector<int> rsv = rs.getshuffled();
    std::vector<int>::iterator it;

    //Execute in 1 epoch
    for(it = rsv.begin(); it != rsv.end(); ++it){
      dvect vnorm((int)refVector.size1());

      if(broken_neuron_index.size() == 0){
	for(int i = 0; i < (int)refVector.size1(); ++i){
	  vnorm(i) = norm_2(row(*inputVector, *it) - row(refVector, i));
	}
      }else{
	for(int i = 0; i < (int)refVector.size1(); ++i){
	  if(broken_neuron_flag.at(i) == 0){
	    vnorm(i) = norm_2(row(*inputVector, *it) - row(refVector, i));
	  }else{
	    vnorm(i) = DBL_MAX;
	  }
	}
      }

      int min_index = min_element(vnorm.begin(), vnorm.end()).index();

      Rc(0) = min_index - floor((double)(min_index) / (double)(xlength)) * ylength;
      Rc(1) = floor((double)(min_index) / (double)(xlength));

      for(int y = 0; y < ylength; ++y){
	for(int x = 0; x < xlength; ++x){
	  int const coord = ylength * y + x;
	  R(0) = x;
	  R(1) = y;
	  h = eta * exp(-1.0 * norm_2(Rc - R) * norm_2(Rc - R) / sigma);

	  row(refVector, coord) = row(refVector, coord) + h * (row(*inputVector, *it) - row(refVector, coord));
	}
      }
      sum_err += norm_2(row(*inputVector, *it) - row(refVector, min_index)) * norm_2(row(*inputVector, *it) - row(refVector, min_index));
    }
    
    Error.push_back(sum_err / (double)msize);
    write_map(teacher, t + 1, directory);
  }

  cout << "SOM has finished to learn." << endl;

  return Error;
}
int som::size(){
  return (xlength * ylength);
}

dmat som::getRefVector(){
  return refVector;
}

dvect som::getCoord(dmat_r inputVector_row){
  int min_index = 0;
  dvect coord(2);
  dvect vnorm((int)refVector.size1());

  if(broken_neuron_index.size() == 0){
    for(int i = 0; i < (xlength * ylength); ++i){
      vnorm(i) = norm_2(inputVector_row - row(refVector, i));
    }
  }else{
    for(int i = 0; i < (xlength * ylength); ++i){
      if(broken_neuron_flag.at(i) == 0){
	vnorm(i) = norm_2(inputVector_row - row(refVector, i));
      }else{
	vnorm(i) = DBL_MAX;
      }
    }
  }
  
  min_index = min_element(vnorm.begin(), vnorm.end()).index();

  if(dim == 1){
    coord(0) = min_index;
    coord(1) = 1;
  }else{
    coord(0) = min_index - floor(static_cast<double>(min_index) / static_cast<double>(xlength)) * ylength;
    coord(1) = floor(static_cast<double>(min_index) / static_cast<double>(xlength));
  }
  
  return coord;
}

dvect som::getCoord(dvect inputVector){
  dvect coord(2);
  int min_index = 0;
  dvect vnorm((int)refVector.size1());

  if(broken_neuron_index.size() == 0){
    for(int i = 0; i < (xlength * ylength); ++i){
      vnorm(i) = norm_2(inputVector - row(refVector, i));
    }
  }else{
    for(int i = 0; i < (xlength * ylength); ++i){
      if(broken_neuron_flag.at(i) == 0){
	vnorm(i) = norm_2(inputVector - row(refVector, i));
      }else{
	vnorm(i) = DBL_MAX;
      }
    }
  }

  min_index = min_element(vnorm.begin(), vnorm.end()).index();

  if(dim == 1){
    coord(0) = min_index;
    coord(1) = 1;
  }else{
    coord(0) = min_index - floor(static_cast<double>(min_index) / static_cast<double>(xlength)) * ylength;
    coord(1) = floor(static_cast<double>(min_index) / static_cast<double>(xlength));
  }
  
  return coord;
}
void som::broken_neuron(double const rate){
  rnd_shuffle rs(xlength * ylength);
  std::vector<int> rsv = rs.getshuffled();
  std::vector<int>::iterator it = rsv.begin();

  broken_neuron_index.resize((double)(xlength * ylength) * 0.01 * rate);
  broken_neuron_flag.resize(xlength * ylength);
  if(rate < 100){
    for(int i = 0, i_max = (int)broken_neuron_index.size(); i < i_max; ++i,++it){
      broken_neuron_index.at(i) = *it;
    }
    sort(broken_neuron_index.begin(), broken_neuron_index.end());
  }else{
    for(int i = 0, i_max = (int)broken_neuron_index.size(); i < i_max; ++i){
      broken_neuron_index.at(i) = i;
    }
  }

  for(int i = 0, cnt = 0, i_max = (int)broken_neuron_flag.size() ; i < i_max; ++i){
    if(cnt < (int)broken_neuron_index.size() && broken_neuron_index.at(cnt) == i){
      broken_neuron_flag.at(i) = 1;
      ++cnt;
    }else{
      broken_neuron_flag.at(i) = 0;
    }
  }
}
void som::broken_neuron(dvect const vector){
  int min_index = 0;
  dvect vnorm((int)refVector.size1());

  if(broken_neuron_index.size() == 0){
    for(int i = 0; i < (xlength * ylength); ++i){
      vnorm(i) = norm_2(vector - row(refVector, i));
    }
  }else{
    for(int i = 0; i < (xlength * ylength); ++i){
      if(broken_neuron_flag.at(i) == 0){
	vnorm(i) = norm_2(vector - row(refVector, i));
      }else{
	vnorm(i) = DBL_MAX;
      }
    }
  }

  min_index = min_element(vnorm.begin(), vnorm.end()).index();

  broken_neuron_index.push_back(min_index);
  broken_neuron_flag.at(min_index) = 1;
}
void som::setTeacher(dmat *t){
  som::teacher = t;
}
void som::setHash(){
  int count = 1;

  str_hash.clear();
  for(int i = 0; i < (int)inputVector->size1(); ++i){
    dmat_r x_row(*inputVector, i);
    dmat_r t_row(*teacher, i);
    
    if(i > 0){
      if((int)norm_2(t_row - row(*teacher, i - 1)) > 0){
	++count;
      }
    }

    dvect w_coord = som::getCoord(x_row);

    try{
      if(str_hash[lexical_cast<string>(w_coord)] != count && str_hash[lexical_cast<string>(w_coord)] != 0){
	str_hash.erase(lexical_cast<string>(w_coord));
	str_hash.insert(map<string, int>::value_type(lexical_cast<string>(w_coord), -1));
      }else{
	str_hash.erase(lexical_cast<string>(w_coord));
	str_hash.insert(map<string, int>::value_type(lexical_cast<string>(w_coord), count));
      }
    }catch(char *e){
      throw  "Exception : part of str_hash_init";
    }
  }
}

void som::setHash(dmat *t){
  som::teacher = t;
  int count = 1;

  for(int i = 0, i_max = (int)inputVector->size1(); i < i_max; ++i){
    dmat_r x_row(*inputVector, i);
    dmat_r t_row(*teacher, i);
    
    if(i > 0){
      if((int)norm_2(t_row - row(*teacher, i - 1)) > 0){
	++count;
      }
    }

    dvect w_coord = som::getCoord(x_row);

    try{
      if(str_hash[lexical_cast<string>(w_coord)] != count && str_hash[lexical_cast<string>(w_coord)] != 0){
	str_hash.erase(lexical_cast<string>(w_coord));
	str_hash.insert(map<string, int>::value_type(lexical_cast<string>(w_coord), -1));
      }else{
	str_hash.erase(lexical_cast<string>(w_coord));
	str_hash.insert(map<string, int>::value_type(lexical_cast<string>(w_coord), count));
      }
    }catch(char *e){
      cout << "Exception : part of str_hash_init" << endl;
    }
  }
}

map<string, int> som::getHash(){
  return str_hash;
}
std::vector<int> som::getBNIndexList(){
  return broken_neuron_index;
}
bool som::write_map(string label, dmat x, som s, simap str_hash){
  for(int i = 0; i < (int)x.size1(); ++i){
    dvect w_coord = s.getCoord(row(x, i));
    int h = str_hash[lexical_cast<string>(w_coord)];
    string fname = "/Users/tkm/programming/c/feature/fmap_class=" + lexical_cast<string>(h) + label + ".dat";
 
    ofstream fs(fname.c_str(), ios::out|ios::app);
    fs << w_coord(0) << " " << w_coord(1) << endl;
    fs.close();
  }

  return true;
}
void som::write_map(dmat const *t, int const lt, string const directory){
  int const msize = som::xlength * som::ylength;
  bool *used_flag = new bool[msize];
  int cnt_it = 1;
  string file_path = "";
  ofstream ofs;
  std::vector<std::vector<dvect> > classification_vector;
  map<string, int> teacher_map;
  dvect w_coord(2);

  try{
    for(int i = 0, i_max = xlength * ylength; i < i_max; ++i){
      used_flag[i] = false;
    }
  }catch(char *e){
    throw "This program failed to initialize used_flag.\n";
  }

  try{
    teacher_map.insert(map<string, int>::value_type(lexical_cast<string>(row(*t, 0)), 1));
    for(int i = 1, cnt = 2, i_max = (int)t->size1(); i < i_max; ++i){
      string key =  lexical_cast<string>(row(*t, i));
      if(teacher_map[key] == 0){
	teacher_map[key] = cnt;
	++cnt;
      }    
    }
  }catch(char *e){
    throw "This program failed to make teacher_map.\n";
  }
  
  try{
    classification_vector.reserve((int)teacher_map.size() + 1);
    for(int i = 0, i_max = (int)teacher_map.size(), reserve_size = (int)(inputVector->size1() / i_max); i <= i_max; ++i){
      std::vector<dvect> tmp;
      
      tmp.reserve(reserve_size);
      classification_vector.push_back(tmp);
    }
  }catch(char *e){
    throw "This program failed to mkae classification_vector.\n";
  }
  
  for(int i = 0, i_max = (int)inputVector->size1(); i < i_max; ++i){
    w_coord = som::getCoord(row(*inputVector, i));
    if(used_flag[(int)(w_coord(0) + w_coord(1) * ylength)] == true){
      classification_vector.at(0).push_back(w_coord);
    }else{
      ++used_flag[(int)(w_coord(0) + w_coord(1) * ylength)];
      try{
	classification_vector.at(teacher_map[lexical_cast<string>(row(*t, i))]).push_back(w_coord);
      }catch(char *e){
	throw "classification_vector[" + lexical_cast<string>(teacher_map[lexical_cast<string>(row(*t, i))]) + " is out of bound.\n";
      }
    }
  }

  try{
    for(std::vector<std::vector<dvect> >::iterator cit = classification_vector.begin(), cit_end = classification_vector.end(); cit != cit_end; ++cit){
      file_path = directory + "map(neuron_num=" + lexical_cast<string>(msize) + ",lt=" + lexical_cast<string>(lt) + ",class=" + lexical_cast<string>(cnt_it) + ").dat";
      ofs.open(file_path.c_str());

      for(std::vector<dvect>::iterator it = cit->begin(), it_end = cit->end(); it != it_end; ++it){
	ofs << (*it)(0) << "\t" << (*it)(1) << endl;
      }
      
      ofs.close();
      ++cnt_it;
    }
  }catch(char *e){
    throw "This program failed to write file.\n";
  }

  delete [] used_flag;
}

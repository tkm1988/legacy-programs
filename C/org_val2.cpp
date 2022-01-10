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
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include "data.h"
#include "som.hpp"
#include "rnd_shuffle.h"
#include "k-means.hpp"

using namespace std;
using namespace boost;
using namespace boost::numeric::ublas;

typedef matrix<double> dmat;
typedef matrix_row<dmat> dmat_r;
typedef boost::numeric::ublas::vector<double> boost_dvect;
typedef list<double> dlist;
typedef std::vector<boost_dvect> stl_dvvect;
typedef struct{
  double dist;
  int point1;
  int point2;
}DATA;

int main(int, char**);
double factorial(int);
double combination(int, int);
int compare(const void*, const void*);
double dist(boost_dvect, boost_dvect);

bool LessDist(const DATA &rLeft, const DATA &rRight){
  return rLeft.dist < rRight.dist;
}

int main(int argc, char** argv){
  //Transact for command line argments
  int k = 0;
  if(argc > 1 && argc <= 2){
    k = (int)strtol(argv[1], NULL, 10);
    if(k <= 0){
      cout << "Error : argv[1] is out of bounds." << endl;
      exit(-1);
    }
  }else{
    cout << "Arguments Error: ex)>./[program name] [value (integer)]" << endl;
    exit(-1);
  }
  
  //Constant
  int const learning_time = 100;
  int const dimension = 2;
  int const max_sample = 100;
  double const eta = 0.1;
  string const fname = "iris";
  string const extension = ".dat";
  string const fpath = "dataset/" + fname + extension;
  string const fname_err = "error/(kmeans)error_lt" + lexical_cast<string>(learning_time) + "_k" + lexical_cast<string>(k) + "_" + fname + extension;
  //string const fname_err = "error/(kmeanspp)error_lt" + lexical_cast<string>(learning_time) + "_k" + lexical_cast<string>(k) + "_" + fname + extension;
  string const fname_map = "_fmap_lt" + lexical_cast<string>(learning_time) + "_k" + lexical_cast<string>(k) + "_" + fname;
  
  //Variables
  //Loop-Counter
  int sample = 0;
  
  //Result Variable
  int match = 0;
  
  //For using SOM
  dmat x = data::generator(fpath, 4);
  dmat t = data::generator(fpath, 4, 6);
  std::vector<dlist> errVector;
  ofstream f(fname_err.c_str(), ios::out);
  
  //Distance (STRUCTURE)
  DATA iris_dist_data[(int)combination(k, 2)];
  DATA som_dist_data[(int)combination(k, 2)];

  //std::vector<DATA> original_data_dist;
  //std::vector<DATA> som_data_dist;
  
  //*Transaction****************************************************
  for(; sample < max_sample; ++sample){
    cout << "SAMPLE : " << sample + 1 << "/" << max_sample << endl;

    //STL's Vector (Contain Represent Vectors)
    stl_dvvect iris_rp_vect = kmeans(k, &x, STANDARD_K_MEANS);
    //stl_dvvect iris_rp_vect = kmeans(k, &x, K_MEANS_PLAS_PLAS);
    stl_dvvect som_rp_vect;
   
    //Create the instance of Self-Organization Map
    som s = som();
    errVector.push_back(s.Learning(&x, learning_time, dimension, eta));
    
    //Check All Correspondence Relationship
    for(int i = 0; i < (int)iris_rp_vect.size(); ++i){
      som_rp_vect.push_back(s.getCoord(iris_rp_vect.at(i)));
    }

    //Compute All Distance
    for(int i = 0, offset = 1, count = 0; i < (int)iris_rp_vect.size(); ++i, ++offset){
      //DATA tmp;
      boost_dvect v1 = iris_rp_vect.at(i);
      boost_dvect v2 = iris_rp_vect.at(i);
      for(int j = offset; j < (int)iris_rp_vect.size(); ++j, ++count){
	//data
	iris_dist_data[count].dist = norm_2(v1 - iris_rp_vect.at(j));
	iris_dist_data[count].point1 = i;
	iris_dist_data[count].point2 = j;

	//SOM
	som_dist_data[count].dist = norm_2(v2 - som_rp_vect.at(j));
	som_dist_data[count].point1 = i;
	som_dist_data[count].point2 = j;
	/*
	tmp.dist = norm_2(v1 - iris_rp_vect.at(j));
	tmp.point1 = i;
	tmp.point2 = j;
	original_data_dist.push_back(tmp);
	tmp.dist = norm_2(v2 - som_rp_vect.at(j));
	tmp.point1 = i;
	tmp.point2 = j;
	som_data_dist.push_back(tmp);
	*/
      }
    }
    
    //Quick Sort
    qsort(iris_dist_data, (int)(combination(k, 2)), sizeof(iris_dist_data[0]), compare);
    qsort(som_dist_data, (int)(combination(k, 2)), sizeof(som_dist_data[0]), compare);
    
    //Compare
    for(int i = 0; i < (int)(combination(k, 2)); ++i){
      cout << "Data " << iris_dist_data[i].point1 << " - " << iris_dist_data[i].point2 << ", "\
      << "SOM " << som_dist_data[i].point1 << " - " << som_dist_data[i].point2 << endl;
      
      if(iris_dist_data[i].point1 == som_dist_data[i].point1 && iris_dist_data[i].point2 == som_dist_data[i].point2){
	++match;
	//}else if(iris_dist_data[i].point1 == som_dist_data[i].point2 && iris_dist_data[i].point2 == som_dist_data[i].point1){
	//++match;
	//}else{
      }
    }
    if((sample+1) == max_sample){
      for(int i = 0; i < (int)som_rp_vect.size(); ++i){
	cout << som_rp_vect.at(i) << endl;
      }

      s.setHash(t);

      data::write_map(fname_map, x, s, s.getHash());
    }
  }

  cout << "Organization : " << match << " / " << (double)max_sample * factorial(k)/(factorial(2)*factorial(k - 2)) << " = " << (double)match / ((double)max_sample * factorial(k)/(factorial(2)*factorial(k - 2))) << endl;
  
  for(int i = 0; i < learning_time; ++i){
    double cumulo_error = 0.0;
    std::vector<dlist>::iterator it = errVector.begin();
    for(int j = 0; j < max_sample; ++j, ++it){
      cumulo_error += *it->begin();
      it->pop_front();
    }
    f << cumulo_error / max_sample << std::endl;
  }
  
  return 0;
}

//There is the function "compare" to use the function "qsort".
//This compares a distance value with another one.
//This returns "1", if distance value of "a" is bigger than another one.
//This returns "0", if distance value of "a" equals another one.
//This returns "-1", if distance value of "a" is smaller than another one.
int compare(const void* a, const void* b){
  DATA target1 = *(DATA *)a;
  DATA target2 = *(DATA *)b;
  if(target1.dist > target2.dist){
    return 1;
  }else if(target1.dist == target2.dist){
    return 0;
  }else{
    return -1;
  }
}

//The function "factorial" provide  a function to compute factorial.
double factorial(int num){
  double f = 1;
  for(int i = num; i > 0; --i){
    f *= i;
  }
  return f;
}

double dist(boost_dvect v1, boost_dvect v2){
  return norm_2(v1 - v2);
}

//The function "combination" provide a function to compute combination
double combination(int n, int m){
  return  factorial(n)/(factorial(m)*factorial(n - m));
}

#pragma once

#include <cmath>
#include <ctime>
#include <list>
#include <map>
#include <boost/random.hpp>
#include <boost/numeric/ublas/matrix.hpp> 
#include <boost/numeric/ublas/vector.hpp> 
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include "rnd_shuffle.h"

using namespace std;
using namespace boost;
using namespace numeric::ublas;

typedef matrix<double> dmat;
typedef matrix_row<matrix<double> > dmat_r;
typedef boost::numeric::ublas::vector<double> dvect;


class som{
private:
	dmat *inputVector;
	dmat refVector;
	map<string, int> str_hash;
	int learning_time;
	int xlength;
	int ylength;
	int dim;
	double init_eta;
	double init_sigma;

public:
	som(void);
	som(dmat *);
	std::list<double> Learning(dmat *, int, int, double);
	std::list<double> Learning(int, int, double);
	int size(void);
	dmat getRefVector();
	dmat getCoord(dmat_r);
	dvect getCoord(dvect);
	void setHash(dmat);
	map<string, int> getHash();
};

som::som(){
	learning_time = 0;
	xlength = 0;
	ylength = 0;
	dim = 0;
	init_eta = 0.0;
	init_sigma = 0.0;
}

som::som(dmat *inputVector){
	this->inputVector = inputVector;

	int inputVectorRow =  static_cast<int>(inputVector->size1());
	int inputVectorColumn = static_cast<int>(inputVector->size2());
	//Melsen Twister Generater
	mt19937 gen(static_cast<unsigned long>(time(0)));
	uniform_smallint<> dst(1, 10);
	variate_generator<mt19937, uniform_smallint<> > rand(gen, dst);

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
		if(fmod(sqrt((double)inputVectorRow), 1.0) == 0){
			som::xlength = static_cast<int>(std::sqrt(static_cast<double>(inputVectorRow)));
			som::ylength = som::xlength;
			som::refVector.resize((som::xlength*som::ylength), inputVectorColumn);

			for(int i = 0; i < (som::xlength*som::ylength); ++i){
				for(int j = 0; j < inputVectorColumn; ++j){
					refVector(i, j) = 0.01 * rand();
				}
			}
		}else{
/*
			som::xlength = (int)std::floor(std::sqrt((double)inputVectorRow)) + 1;
			som::ylength = som::xlength;
*/		
			som::xlength = 10;
			som::ylength = 10;

			som::refVector.resize((som::xlength*som::ylength), inputVectorColumn);

			for(int i = 0; i < (som::xlength*som::ylength); ++i){
				for(int j = 0; j < inputVectorColumn; ++j){
					refVector(i, j) = 0.01 * rand();
				}
			}
		}
	}
}

list<double> som::Learning(dmat *inputVector, int learning_time, int dim, double init_eta){
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
	  //FIXED map's size
	  som::xlength = 10;
	  som::ylength = 10;
	  
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

	//*Learning Part***************************************************************************
	for(int t = 0; t < som::learning_time; ++t){
	  cout << t + 1<< std::endl;
	  double sum_err = 0.0;
	  sigma = init_sigma * pow(0.01 / init_sigma, (double)(t + 1)/som::learning_time);
	  rnd_shuffle rs(inputVectorRow);
	  std::vector<int> rsv = rs.getshuffled();
	  std::vector<int>::iterator it;
	  
	  for(it = rsv.begin(); it != rsv.end(); ++it){
	    //*Compete Part***********************************************
	    list<double> all_dist;
	    list<double>::iterator list_it;
	    dmat_r imr(*inputVector, *it);
	    int min_index = 0;
	    double min_value = 0.0;
	    
	    //Compute the Eucridian Distance
	    for(int i = 0; i < (xlength * ylength); ++i){
	      dmat_r rmr(refVector, i);
	      all_dist.push_back(norm_2(rmr - imr)*norm_2(rmr - imr));
	    }
	    min_value = *std::min_element(all_dist.begin(), all_dist.end());
	    
	    list_it = all_dist.begin();
	    for(int i = 0; list_it != all_dist.end(); ++i, ++list_it){
	      if(*list_it == min_value){
		min_index = i;
		break;
	      }
	    }
	    //*Renewal Part***********************************************
	    //***1D***(NOT TEST)
	    if(som::dim == 1){
	      for(int x = 0; x < (xlength * ylength); ++x){
		double h = eta * std::exp(-1.0 * (x - min_index )*(x - min_index ) / sigma );
		
		dmat_r rmr(refVector, x);
		
		rmr = rmr + h * (imr - rmr);
	      }
	      //***2D***
	    }else{
	      dmat Rc(1, 2);
	      Rc(0, 0) = min_index - std::floor(static_cast<double>(min_index) / static_cast<double>(xlength)) * ylength;
	      Rc(0, 1) = std::floor(static_cast<double>(min_index) / static_cast<double>(xlength));
	      dmat_r rcr(Rc, 0);
	      
	      for(int y = 0; y < ylength; ++y){
		for(int x = 0; x < xlength; ++x){
		  dmat R(1, 2);
		  R(0, 0) = x;
		  R(0, 1) = y;
		  dmat_r rr(R, 0);
		  dmat_r rmr(refVector, (x + y * xlength));
		  
		  //double h = eta * std::exp(-1.0 * norm_2(rr - rcr)* norm_2(rr - rcr) / (2*sigma*sigma));
		  double h = eta * std::exp(-1.0 * norm_2(rr - rcr)*norm_2(rr - rcr) / sigma);
		  
		  rmr = rmr + h * (imr - rmr);
		}
	      }
	    }
	    dmat_r rmr(refVector, static_cast<unsigned int>(min_index));
	    sum_err = sum_err + norm_2(imr - rmr) * norm_2(imr - rmr);
	  }
	  //eta =  init_eta - init_eta * std::exp(-5.0 * init_eta * (t + 1) / som::learning_time);
	  //eta =  init_eta * std::exp(-2.0 * init_eta * (t + 1) / som::learning_time);
	  //eta =  init_eta * (1.0 -  (t + 1) / som::learning_time);
	  //sigma = init_sigma * std::exp(-2.0 * init_eta * (t + 1) / som::learning_time);
	  Error.push_back(sum_err / (xlength * ylength));
	}
	
	return Error;
}

list<double> som::Learning(int learning_time, int dim, double init_eta){
	som::learning_time = learning_time;
	som::init_eta = init_eta;
	som::dim = dim;

	std::list<double> Error;
	double eta = som::init_eta;
	int inputVectorRow =  static_cast<int>(inputVector->size1());
	//int inputVectorColumn = static_cast<int>(inputVector->size2());

	//som::init_sigma = (double)xlength / 2.0;
	som::init_sigma = 20.0;
	double sigma = som::init_sigma;

	//Learning Part
	std::cout << "Learning Time (SOM)" << std::endl;
	for(int t = 0; t < som::learning_time; ++t){
		cout << t + 1 << endl;
		double sum_err = 0.0;
		sigma = init_sigma * std::pow(0.01 / init_sigma, (double)(t + 1)/som::learning_time);
		rnd_shuffle rs(inputVectorRow);
		std::vector<int> rsv = rs.getshuffled();
		std::vector<int>::iterator it;

		for(it = rsv.begin(); it != rsv.end(); ++it){
		//*Compete Part***********************************************
			list<double> all_dist;
			list<double>::iterator list_it;
			dmat_r imr(*inputVector, *it);
			int min_index = 0;
			double min_value = 0.0;

			for(int i = 0; i < (xlength * ylength); ++i){
				dmat_r rmr(refVector, i);
				all_dist.push_back(norm_2(rmr - imr)*norm_2(rmr - imr));
			}
			min_value = *std::min_element(all_dist.begin(), all_dist.end());
			
			list_it = all_dist.begin();
			for(int i = 0; list_it != all_dist.end(); ++i, ++list_it){
				if(*list_it == min_value){
					min_index = i;
					break;
				}
			}
		//*Renewal Part***********************************************
			if(som::dim == 1){
				for(int x = 0; x < (xlength * ylength); ++x){
					double h = eta * std::exp(-1.0 * (x - min_index )*(x - min_index ) / sigma );

					dmat_r rmr(refVector, x);

					rmr = rmr + h * (imr - rmr);
				}
			}else{
				dmat Rc(1, 2);
				Rc(0, 0) = min_index - std::floor(static_cast<double>(min_index) / static_cast<double>(xlength)) * ylength;
				Rc(0, 1) = std::floor(static_cast<double>(min_index) / static_cast<double>(xlength));
				dmat_r rcr(Rc, 0);
				
				for(int y = 0; y < ylength; ++y){
					for(int x = 0; x < xlength; ++x){
						dmat R(1, 2);
						R(0, 0) = x;
						R(0, 1) = y;
						dmat_r rr(R, 0);
						dmat_r rmr(refVector, (x + y * xlength));

						//double h = eta * std::exp(-1.0 * norm_2(rr - rcr)* norm_2(rr - rcr) / (2*sigma*sigma));
						double h = eta * std::exp(-1.0 * norm_2(rr - rcr)*norm_2(rr - rcr) / sigma);

						rmr = rmr + h * (imr - rmr);
					}
				}
			}
			dmat_r rmr(refVector, static_cast<unsigned int>(min_index));
			sum_err = sum_err + norm_2(imr - rmr) * norm_2(imr - rmr);
		}
		//eta =  init_eta - init_eta * std::exp(-5.0 * init_eta * (t + 1) / som::learning_time);
		//eta =  init_eta * std::exp(-2.0 * init_eta * (t + 1) / som::learning_time);
		//eta =  init_eta * (1.0 -  (t + 1) / som::learning_time);
		//sigma = init_sigma * std::exp(-2.0 * init_eta * (t + 1) / som::learning_time);
		Error.push_back(sum_err / (xlength * ylength));
	}

	return Error;
}

int som::size(){
	return (xlength * ylength);
}

dmat som::getRefVector(){
	return refVector;
}

dmat som::getCoord(dmat_r inputVector_row){
	dmat coord(1, 2);
	list<double> all_dist;
	list<double>::iterator list_it;
	int min_index = 0;
	double min_value = 0.0;

	for(int i = 0; i < (xlength * ylength); ++i){
		dmat_r rmr(refVector, i);
		all_dist.push_back(norm_2(inputVector_row - rmr)*norm_2(inputVector_row - rmr));
	}
	min_value = *min_element(all_dist.begin(), all_dist.end());

	list_it = all_dist.begin();
	for(int i = 0; list_it != all_dist.end(); ++i, ++list_it){
		if(*list_it == min_value){
			min_index = i;
			break;
		}
	}


	if(dim == 1){
		coord(0, 0) = min_index;
		coord(0, 1) = 1;
	}else{
		coord(0, 0) = min_index - floor(static_cast<double>(min_index) / static_cast<double>(xlength)) * ylength;
		coord(0, 1) = floor(static_cast<double>(min_index) / static_cast<double>(xlength));
	}

	return coord;
}

dvect som::getCoord(dvect inputVector){
	dvect coord(2);
	list<double> all_dist;
	list<double>::iterator list_it;
	int min_index = 0;
	double min_value = 0.0;

	for(int i = 0; i < (xlength * ylength); ++i){
		dmat_r rmr(refVector, i);
		all_dist.push_back(norm_2(inputVector - rmr)*norm_2(inputVector - rmr));
	}
	min_value = *min_element(all_dist.begin(), all_dist.end());

	list_it = all_dist.begin();
	for(int i = 0; list_it != all_dist.end(); ++i, ++list_it){
		if(*list_it == min_value){
			min_index = i;
			break;
		}
	}


	if(dim == 1){
		coord(0) = min_index;
		coord(1) = 1;
	}else{
		coord(0) = min_index - floor(static_cast<double>(min_index) / static_cast<double>(xlength)) * ylength;
		coord(1) = floor(static_cast<double>(min_index) / static_cast<double>(xlength));
	}

	return coord;
}

void som::setHash(dmat t){
  int count = 1;

  for(int i = 0; i < (int)inputVector->size1(); ++i){
    dmat_r x_row(*inputVector, i);
    dmat_r t_row(t, i);
    
    if(i > 0){
      if((int)norm_2(t_row - row(t, i - 1)) > 0){
	++count;
      }
    }

    dmat w_coord = som::getCoord(x_row);

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

#pragma once

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <boost/numeric/ublas/matrix.hpp> 
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;
using namespace numeric::ublas;

typedef char_separator<char> csc;
typedef tokenizer<csc> tknzr;
typedef map<string, int> simap;

class data{
private:
public:
	static matrix<double> generator(void);
	static matrix<double> generator(std::string, int);
	static matrix<double> generator(std::string, int, int);
	static bool write_list(std::string, std::list<double>);
	static bool write_mat(std::string, matrix<double>);
};
matrix<double> data::generator(){
	matrix<double> x(100, 2);
	int x1 = 0, x2 = 1;

	for(int i = 0; i < 100; ++i){
		for(int j = 0; j < 2; ++j){
			if(j == 0){
				x(i, j) = x1;
			}else{
				x(i, j) = x2++;
			}
		}
		if(((i + 1) % 10) == 0){
			x1++;
			x2 = 1;
		}
	}

	return x;
}
matrix<double> data::generator(std::string fname, int column){
	int x_row = 0, x_col = 0;
	std::string line;
	std::ifstream f;
	std::list<double> num_token;
	std::list<double>::iterator nt_it;
	matrix<double> x;
	csc sep(", \t");

	try{
	  f.open(fname.c_str());
	  while(getline(f, line)){
	    tknzr line_tknzr(line, sep);
	    tknzr::iterator it = line_tknzr.begin();
	    
	    ++x_row;
	    for(int i = 0;(i < column) && (it != line_tknzr.end()); ++i, ++it){
	      num_token.push_back(boost::lexical_cast<double>(*it));
	    }
	  }

	  x_col = num_token.size() / x_row;
	}catch(char *e){
	  cout << "Exception [data.h] L60-72 : " << e << endl; 
	}

	x.resize(x_row, x_col);
	nt_it = num_token.begin();
	for(int i = 0; i < x_row; ++i){
		for(int j = 0; j < x_col; ++j){
			x(i, j) = *nt_it;
			++nt_it;
		}
	}

	f.close();
	return x;
}
matrix<double> data::generator(std::string fname, int begin_column, int end_column){
	int x_row = 0, x_col = 0;
	std::string line;
	std::ifstream f;
	std::list<double> num_token;
	std::list<double>::iterator nt_it;
	matrix<double> x;
	csc sep(", \t");

	try{
	  f.open(fname.c_str());
	  while(getline(f, line)){
	    tknzr line_tknzr(line, sep);
	    tknzr::iterator it = line_tknzr.begin();
	    
	    ++x_row;
	    for(int i = 0; it != line_tknzr.end(); ++i, ++it){
	      if((i >= (begin_column - 1)) && ((i <= (end_column - 1)))){
		//num_token.push_back(boost::lexical_cast<double>(*it));
		num_token.push_back(strtod((*it).c_str(),NULL));
	      }
	    }
	  }
	  
	  x_col = num_token.size() / x_row;
	}catch(char *e){
	  cout << "Exception [data.h] L99-112 : " << e << endl;
	}

	x.resize(x_row, x_col);
	nt_it = num_token.begin();
	for(int i = 0; i < x_row; ++i){
		for(int j = 0; j < x_col; ++j){
			x(i, j ) = *nt_it;
			++nt_it;
		}
	}

	f.close();
	return x;
}
bool data::write_list(std::string fname, std::list<double> lst){
  std::ofstream f(fname.c_str());
	for(std::list<double>::iterator lst_it = lst.begin(); lst_it != lst.end(); ++lst_it){
		f << *lst_it << std::endl;
	}
	f.close();
	return true;
}
bool data::write_mat(std::string fname, matrix<double> mat){
  std::ofstream f(fname.c_str());
	for(int i = 0;  i < (int)mat.size1(); ++i){
		for(int j = 0; j < (int)mat.size2(); ++j){
			f << mat(i, j) << " ";
		}
		f << std::endl;
	}
	f.close();
	return true;
}

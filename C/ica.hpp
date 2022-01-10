#pragma once

#include <cstdlib>
#include <cfloat>
#include <cmath>
#include <iostream>
#include <list>
#include <vector>
#include <map>

#include <itpp/signal/fastica.h>
#include <itpp/itbase.h>

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/numeric/ublas/vector.hpp>

#include "rnd_shuffle.h"

using namespace std;
using namespace itpp;
using namespace boost;
using namespace boost::numeric::ublas;

typedef matrix<double> dmat;
typedef matrix_row<dmat> dmat_r;

dmat ica(){
  
}


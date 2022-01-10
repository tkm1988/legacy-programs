#ifndef UBLASDETERMINANT_HPP_20120210
#define UBLASDETERMINANT_HPP_20120210

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif


#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/matrix.hpp>


namespace ublasdeterminant {
  template <class M> double determinant(const M& m);
}


template <class M>
double ublasdeterminant::determinant(const M& m)
{
  namespace ublas = boost::numeric::ublas;

  BOOST_UBLAS_CHECK(m.size1() == m.size2(), ublas::external_logic());

  ublas::matrix<double>       lu(m);
  ublas::permutation_matrix<> pm(m.size1());

  ublas::lu_factorize(lu, pm);

  double det(1);

  typedef ublas::permutation_matrix<>::size_type size_type;

  for (size_type i = 0; i < pm.size(); ++i) {
    det *= (i == pm(i)) ? +lu(i, i) : -lu(i, i);
  }

  return det;
}


#endif

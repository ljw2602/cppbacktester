//
//  Inversion.hpp
//  cppbacktester
//
//  Created by Jeongwon on 4/23/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#ifndef Inversion_hpp
#define Inversion_hpp

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/io.hpp>

template<class T>
bool InvertMatrix(const boost::numeric::ublas::matrix<T>& input, boost::numeric::ublas::matrix<T>& inverse)
{
    typedef boost::numeric::ublas::permutation_matrix<std::size_t> pmatrix;
    
    // create a working copy of the input
    boost::numeric::ublas::matrix<T> A(input);
    
    // create a permutation matrix for the LU-factorization
    pmatrix pm(A.size1());
    
    // perform LU-factorization
    int res = boost::numeric::ublas::lu_factorize(A, pm);
    if (res != 0)
        return false;
    
    // create identity matrix of "inverse"
    inverse.assign(boost::numeric::ublas::identity_matrix<T> (A.size1()));
    
    // backsubstitute to get the inverse
    boost::numeric::ublas::lu_substitute(A, pm, inverse);
    
    return true;
}

#endif /* Inversion_hpp */

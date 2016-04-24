//
//  BlackLitterman.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/23/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include "BlackLitterman.hpp"


std::vector<double> bl_norm_weight(const std::vector<double>& hist_return_,  // monthly return , annualized
                                   const std::vector<double>& hist_stdev_,  //
                                   const std::vector<double>& market_cap_,
                                   const std::vector<double>& corr_,
                                   const std::vector<double>& p_,
                                   const std::vector<double>& v_,
                                   const std::vector<double>& v_range_,
                                   const std::vector<double>& v_conf_,
                                   double gamma,
                                   double tau)
{
    // Calculates Black-Litterman adjusted expected return
    // and MV optimized portfolio weight based on that
    // The sum of weight is normalized to 100%
    
    typedef boost::numeric::ublas::matrix<double> matrixdb;

    const unsigned n = hist_return_.size();  // # assets
    const unsigned k = v_.size();  // # views
    
    // Map std::vector to boost::ublas::matrix
    matrixdb hist_return(n, 1);
    for (unsigned i = 0; i < hist_return.size1(); ++ i)
        hist_return(i, 0) = hist_return_[i];
    
    matrixdb hist_stdev(n, 1);
    for (unsigned i = 0; i < hist_stdev.size1(); ++ i)
        hist_stdev(i, 0) = hist_stdev_[i];
    
    matrixdb market_cap(n, 1);
    for (unsigned i = 0; i < market_cap.size1(); ++ i)
        market_cap(i, 0) = market_cap_[i];
    
    matrixdb corr(n, n);
    for (unsigned i = 0; i < corr.size1(); ++ i)
        for (unsigned j = 0; j < corr.size2(); ++ j)
            corr(i, j) = corr_[i*corr.size2() + j];
    
    matrixdb p(k, n);
    for (unsigned i = 0; i < p.size1(); ++ i)
        for (unsigned j = 0; j < p.size2(); ++ j)
            p(i, j) = p_[i*p.size2() + j];
    
    matrixdb v(k, 1);
    for (unsigned i = 0; i < v.size1(); ++ i)
        v(i, 0) = v_[i];
    
    matrixdb v_range(k, 1);
    for (unsigned i = 0; i < v_range.size1(); ++ i)
        v_range(i, 0) = v_range_[i];
    
    matrixdb v_conf(k, 1);
    for (unsigned i = 0; i < v_conf.size1(); ++ i)
        v_conf(i, 0) = v_conf_[i];
    
    matrixdb sigma(k, k);
    for (unsigned i = 0; i < sigma.size1(); ++ i) {
        for (unsigned j = 0; j < sigma.size2(); ++ j) {
            if (j == i)
                sigma(i, j) = pow(v_range_[i] / gsl_cdf_ugaussian_Pinv( (1.0+v_conf_[i])/2.0 ), 2);
            else
                sigma(i, j) = 0;
        }
    }
    
    matrixdb one(n, 1);
    for (unsigned i = 0; i < one.size1(); ++ i)
        one(i, 0) = 1.0;
    
    matrixdb sigma_inv(k, k);
    InvertMatrix<double>(sigma, sigma_inv);
    
    matrixdb omega(n, n);
    omega = element_prod(matrixdb(prod(hist_stdev, trans(hist_stdev))),corr);
    
    matrixdb omega_inv(n, n);
    InvertMatrix<double>(omega, omega_inv);
    
    matrixdb tauomega_inv(n, n);
    InvertMatrix<double>(tau*omega, tauomega_inv);
    
    matrixdb gammaomega_inv(n, n);
    InvertMatrix<double>(gamma*omega, gammaomega_inv);
    
    // Start calculating important variables
    matrixdb pi(n, 1);
    pi = gamma*prod(omega,market_cap);
    
    matrixdb ER_BL_first(n, n);
    InvertMatrix<double>(tauomega_inv + prod(matrixdb(prod(trans(p),sigma_inv)), p), ER_BL_first);
    matrixdb ER_BL_second(n, 1);
    ER_BL_second = prod(tauomega_inv, pi) + prod(matrixdb(prod(trans(p),sigma_inv)), v);
    matrixdb ER_BL(n, 1);
    ER_BL = prod(ER_BL_first, ER_BL_second);
    
    matrixdb w_BL(n, 1);
    w_BL = prod(gammaomega_inv, ER_BL);
    
    matrixdb w_BL_norm(n, 1);
    w_BL_norm = prod(omega_inv,one)/(prod(matrixdb(prod(trans(one),omega_inv)), one)(0,0)) + prod(gammaomega_inv, ER_BL - (prod(matrixdb(prod(trans(one),omega_inv)), ER_BL)(0,0))/(prod(matrixdb(prod(trans(one),omega_inv)), one)(0,0))*one);
    
//    std::cout << hist_return << std::endl;
//    std::cout << hist_stdev << std::endl;
//    std::cout << market_cap << std::endl;
//    std::cout << corr << std::endl;
//    std::cout << p << std::endl;
//    std::cout << v << std::endl;
//    std::cout << v_range << std::endl;
//    std::cout << v_conf << std::endl;
//    std::cout << sigma << std::endl;
//    std::cout << omega << std::endl;
//    std::cout << sigma_inv << std::endl;
//    std::cout << tauomega_inv << std::endl;
//    std::cout << pi << std::endl;
//    std::cout << ER_BL << std::endl;
//    std::cout << w_BL << std::endl;
//    std::cout << w_BL_norm << std::endl;
    
    // Export in std::vector
    std::vector<double> norm_weight(n);
    for (unsigned i = 0; i < norm_weight.size(); ++ i)
        norm_weight[i] = w_BL_norm(i,0);
    
    return norm_weight;
}

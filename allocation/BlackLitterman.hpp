//
//  BlackLitterman.hpp
//  cppbacktester
//
//  Created by Jeongwon on 4/23/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#ifndef BlackLitterman_hpp
#define BlackLitterman_hpp

/*
 References:
 [1] Cominvest's Black-Litterman explanation
 */

#include "Inversion.hpp"
#include <vector>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <gsl/gsl_cdf.h>

std::vector<double> bl_norm_weight(const std::vector<double>& hist_return_,
                                   const std::vector<double>& hist_stdev_,
                                   const std::vector<double>& market_cap_,
                                   const std::vector<double>& corr_,
                                   const std::vector<double>& p_,
                                   const std::vector<double>& v_,
                                   const std::vector<double>& v_range_,
                                   const std::vector<double>& v_conf_,
                                   double gamma = 3.0,
                                   double tau = 0.3);

#endif /* BlackLitterman_hpp */

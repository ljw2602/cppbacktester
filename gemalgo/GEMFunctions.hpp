//
//  GEMFunctions.hpp
//  cppbacktester
//
//  Created by Jeongwon on 4/26/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#ifndef GEMFunctions_hpp
#define GEMFunctions_hpp

#include <map>
#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>

#include "DB.hpp"
#include "TA.hpp"
#include "BlackLitterman.hpp"
#include "BalanceSet.hpp"


void preprocess_data(unsigned yr, unsigned mon);

double rate_of_change(const std::string& name, unsigned yr, unsigned mon);

std::map<double, std::string, std::greater<double> > rate_of_change_map(const unsigned lookback_yr, const unsigned lookback_mo);

std::map<double, std::string, std::greater<double> > monthly_estimation(const boost::gregorian::date& today, const unsigned lookback_yr, const unsigned lookback_mo);

std::map<std::string, int> monthly_allocation(const std::map<double, std::string, std::greater<double> >& est, double minimum, const boost::gregorian::date& today);

void date_check(const boost::gregorian::date& today);

void daily_settlement(std::map<std::string, int>& share_book,
                      const double netcash,
                      const boost::gregorian::date& today);

void summary();


#endif /* GEMFunctions_hpp */

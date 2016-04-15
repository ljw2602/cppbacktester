//
//  TreasurySeries.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/9/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include "TreasurySeries.hpp"
#include <numeric>


std::size_t TreasurySeries::load(FileDriver& driver, const std::string& filename, const boost::gregorian::date& begin, const boost::gregorian::date& end) {
    return _load(driver, filename, begin, end);
}


std::shared_ptr<TreasurySeries> TreasurySeries::monthly( void ) const
{
    std::shared_ptr<TreasurySeries> monthly_series(new TreasurySeries(maturity));
    
    // Iterate through all the months in db
    for( boost::gregorian::month_iterator miter(TS::begin()->first); (*miter) < TS::rbegin()->first; ++miter ) {
        
        // Find first and last entry for this month
        TS::const_iterator first_in_month_iter = first_in_month((*miter).year(), (*miter).month());
        if( first_in_month_iter == TS::end() ) {
            std::cerr << "Warning: can't find series bar (BOM) for month of " << (*miter) << std::endl;
            continue;
        }
        
        // Last entry for this month
        TS::const_iterator last_in_month_iter = last_in_month((*miter).year(), (*miter).month());
        if( last_in_month_iter == TS::end() ) {
            std::cerr << "Warning: can't find series bar (EOM) for month of " << (*miter) << std::endl;
            continue;
        }
        
        // Initialize monthly series
        boost::gregorian::date dt = last_in_month_iter->first; // Monthly series dt is last bar in month
        double ret = 0;
        
        // Extract (assume yield = return) during this month
        std::vector<double> ret_mon;
        for( TS::const_iterator iter(first_in_month_iter); iter != TS::end() && iter->first <= last_in_month_iter->first; ++iter ) {
            ret_mon.push_back(iter->second->ret());
        }
        
        ret = std::accumulate(ret_mon.begin(), ret_mon.end(), 0.0) / ret_mon.size() / 12.0;
        
        monthly_series->insert( TS::value_type(dt, std::shared_ptr<USTreasuryYield>(new USTreasuryYield(dt, ret, maturity))) );
    }
    return monthly_series;
}


std::shared_ptr<TreasurySeries> TreasurySeries::weekly( void ) const
{
    std::shared_ptr<TreasurySeries> weekly_series(new TreasurySeries(maturity));
    
    // Iterate through all the weeks in db
    for( boost::gregorian::week_iterator witer(TS::begin()->first); (*witer) < TS::rbegin()->first; ++witer ) {
        
        // Find first and last entry for this week
        TS::const_iterator first_in_week_iter = first_in_week((*witer).year(), (*witer).month(), (*witer).day());
        if( first_in_week_iter == TS::end() ) {
            std::cerr << "Warning: can't find series bar (BOW) for week of " << (*witer) << std::endl;
            continue;
        }
        
        // Last entry for this week
        TS::const_iterator last_in_week_iter = last_in_week((*witer).year(), (*witer).month(), (*witer).day());
        if( last_in_week_iter == TS::end() ) {
            std::cerr << "Warning: can't find series bar (EOW) for week of " << (*witer) << std::endl;
            continue;
        }
        
        // Initialize this weekly series (O,L,H,C,V)
        boost::gregorian::date dt = last_in_week_iter->first; // Key is EOW
        double ret = 0;
        
        // Extract yield (assume yield = return) during this week
        std::vector<double> ret_week;
        for( TS::const_iterator iter(first_in_week_iter); iter != TS::end() && iter->first <= last_in_week_iter->first; ++iter ) {
            ret_week.push_back(iter->second->ret());
        }
        
        ret = std::accumulate(ret_week.begin(), ret_week.end(), 0.0) / ret_week.size() / 52.0;
        
        weekly_series->insert( TS::value_type(dt, std::shared_ptr<USTreasuryYield>(new USTreasuryYield(dt, ret, maturity))) );
    }
    
    return weekly_series;
}
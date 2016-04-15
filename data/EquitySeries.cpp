//
//  EquitySeries.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/9/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include "EquitySeries.hpp"
#include <numeric>


std::size_t EquitySeries::load(FileDriver& driver, const std::string& filename, const boost::gregorian::date& begin, const boost::gregorian::date& end) {
    return _load(driver, filename, begin, end);
}


std::shared_ptr<EquitySeries> EquitySeries::monthly( void ) const
{
    std::shared_ptr<EquitySeries> monthly_series(new EquitySeries);
    
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
        
        // Extract return (1 + r/100) during this month
        std::vector<double> ret_mon;
        for( TS::const_iterator iter(first_in_month_iter); iter != TS::end() && iter->first <= last_in_month_iter->first; ++iter ) {
            ret_mon.push_back(1.0 + (iter->second->ret())/100.0);
        }
        
        ret = ( std::accumulate(ret_mon.begin(), ret_mon.end(), 1.0, std::multiplies<double>()) - 1.0 ) * 100.0;
        
        monthly_series->insert( TS::value_type(dt, std::shared_ptr<USEquityReturn>(new USEquityReturn(dt, ret))) );
    }
    return monthly_series;
}


std::shared_ptr<EquitySeries> EquitySeries::weekly( void ) const
{
    std::shared_ptr<EquitySeries> weekly_series(new EquitySeries);
    
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
        
        // Extract return (1 + r/100) during this week
        std::vector<double> ret_week;
        for( TS::const_iterator iter(first_in_week_iter); iter != TS::end() && iter->first <= last_in_week_iter->first; ++iter ) {
            ret_week.push_back(1.0 + (iter->second->ret())/100.0);
        }

        ret = ( std::accumulate(ret_week.begin(), ret_week.end(), 1.0, std::multiplies<double>()) - 1.0 ) * 100.0;

        weekly_series->insert( TS::value_type(dt, std::shared_ptr<USEquityReturn>(new USEquityReturn(dt, ret))) );
    }
    
    return weekly_series;
}
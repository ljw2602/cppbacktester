//
//  PriceSeries.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/9/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include "PriceSeries.hpp"
#include <numeric>


std::size_t PriceSeries::load(FileDriver& driver, const std::string& filename, const boost::gregorian::date& begin, const boost::gregorian::date& end) {
    return _load(driver, filename, begin, end);
}


std::shared_ptr<PriceSeries> PriceSeries::monthly( void ) const
{
    std::shared_ptr<PriceSeries> monthly_series(new PriceSeries);
    
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
        
        // Initialize monthly series (O,L,H,C,V)
        boost::gregorian::date dt = last_in_month_iter->first; // Monthly series dt is last bar in month
        double open = first_in_month_iter->second->open(); // Open on first day of the month
        double close = last_in_month_iter->second->close(); // Close on last day of the month
        double adjclose = last_in_month_iter->second->adjclose(); // Adj. close on last day of the month
        double high = 0;
        double low = 0;
        unsigned long volume = 0;
        
        // Store all highs, lows, volume to determine monthly values
        std::vector<double> highs;
        std::vector<double> lows;
        std::vector<unsigned long> volumes;
        for( TS::const_iterator iter(first_in_month_iter); iter != TS::end() && iter->first <= last_in_month_iter->first; ++iter ) {
            highs.push_back((*iter).second->high());
            lows.push_back((*iter).second->low());
            volumes.push_back((*iter).second->volume());
        }
        
        high = (highs.size() > 1 ? *max_element(highs.begin(), highs.end()) : first_in_month_iter->second->high() );
        low = (highs.size() > 1 ? *min_element(lows.begin(), lows.end()) : first_in_month_iter->second->low() );
        
        if( !volumes.empty() )
            volume = std::accumulate<std::vector<unsigned long>::const_iterator, unsigned long>(volumes.begin(), volumes.end(), 0);

        monthly_series->insert( TS::value_type(dt, std::shared_ptr<YahooPrice>(new YahooPrice(dt, open, high, low, close, adjclose, volume))) );
    }
    return monthly_series;
}


std::shared_ptr<PriceSeries> PriceSeries::weekly( void ) const
{
    std::shared_ptr<PriceSeries> weekly_series(new PriceSeries);
    
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
        double open = first_in_week_iter->second->open(); // Open on first day of the week
        double close = last_in_week_iter->second->close(); // Close on last day of the week
        double adjclose = last_in_week_iter->second->adjclose(); // Adj. close on last day of the week
        double high = 0;
        double low = 0;
        unsigned long volume = 0;
        
        // Store all highs, lows, volume to determine weekly values
        std::vector<double> highs;
        std::vector<double> lows;
        std::vector<unsigned long> volumes;
        for( TS::const_iterator iter(first_in_week_iter); iter != TS::end() && iter->first <= last_in_week_iter->first; ++iter ) {
            highs.push_back(iter->second->high());
            lows.push_back(iter->second->low());
            volumes.push_back(iter->second->volume());
        }
        
        high = (highs.size() > 1 ? *max_element(highs.begin(), highs.end()) : first_in_week_iter->second->high() );
        low = (highs.size() > 1 ? *min_element(lows.begin(), lows.end()) : first_in_week_iter->second->low() );
        
        if( !volumes.empty() )
            volume = std::accumulate<std::vector<unsigned long>::const_iterator, unsigned long>(volumes.begin(), volumes.end(), 0);
        
        weekly_series->insert( TS::value_type(dt, std::shared_ptr<YahooPrice>(new YahooPrice(dt, open, high, low, close, adjclose, volume))) );
    }
    
    return weekly_series;
}

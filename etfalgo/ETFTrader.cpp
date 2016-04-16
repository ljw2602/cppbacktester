//
//  ETFTrader.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/14/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include "ETFTrader.hpp"
#include "TA.hpp"


ETFTrader::ETFTrader(const std::string& initial): initial_capital(initial) { }


void preprocess_data(unsigned yr, unsigned mon)
{
    // Advance all data pointers by yr and mon
    // DB::advance() checks data is longer than requested duration

    DB::instance().advance(yr, mon);
}


double yoy_return(const std::string& name)
{
    // Calculates one year return using adjusted close price
    
    TA ta;
    PriceSeries series = DB::instance().get(name).monthly();
    PriceSeries::const_iterator pSeries = DB::instance().get(name).pMonthly();
    TA::ROCRes resROC = ta.ROC(series.adjclose(pSeries,13), 12);
    
    return resROC.roc[0];
}

std::map<double, std::string, std::greater<double> > yoy_return_map()
{
    // Calculates one year return using adjusted close price for all symbols in DB
    
    std::map<double, std::string, std::greater<double> > ret;  // map with descending order
    
    std::vector<std::string> symbols = DB::instance().dblist();
    for (std::vector<std::string>::const_iterator it = symbols.begin(); it != symbols.end(); it++ )
        ret.insert( std::pair<double, std::string>(yoy_return(*it), *it) );
    
    return ret;
}

std::map<double, std::string, std::greater<double> > estimation()
{
    //
    
    std::map<double, std::string, std::greater<double> > ret = yoy_return_map();
    return ret;
//    for (std::map<double, std::string>::const_iterator it = ret.begin(); it != ret.end(); it++) {
//        std::cout << it->second << " " << it->first << std::endl;
//    }
}


void allocation()
{
    /*
     1. Allocate asset weight based on the estimation
     2. Use Black-Litterman
     */
}


void date_check(const boost::gregorian::date& today)
{
    // Check if all DB have data on the specified date

    std::vector<std::string> symbols = DB::instance().dblist();
    for (std::vector<std::string>::const_iterator it = symbols.begin(); it != symbols.end(); it++ ) {
        if (DB::instance().get(*it).pDaily()->first != today)
            throw TraderException(*it + " does not have data on " + to_simple_string(today));
    }
}


void daily_execution()
{
    // Flush all execution orders from prior EOD

}


void daily_settlement()
{
    /*
     1. EOD balance check
     2. EOD split & dividend recognition
     */
}


void summary()
{
    
}


void ETFTrader::run() throw(TraderException)
{
    //
    
    preprocess_data(1,0);
    
    const std::vector<boost::gregorian::date> dt_day = DB::instance().daily_dt();
    const std::vector<boost::gregorian::date> dt_mon = DB::instance().monthly_dt();
    std::vector<boost::gregorian::date>::const_iterator pDay = dt_day.begin();
    std::vector<boost::gregorian::date>::const_iterator pMon = dt_mon.begin();
    
    std::map<std::string, unsigned long> share_book, order_book;
    
    for (; pDay != dt_day.end(); pDay++) {
        boost::gregorian::date today = *pDay;
        
        date_check(today);
        
        daily_execution();
        
        if (today == *pMon) {
//            std::cout << today << " estimation result" << std::endl;
            
            std::map<double, std::string, std::greater<double> > ret = estimation();
            
            allocation();
            
            pMon++;
            DB::instance().monthly_advance();
        }
        
        daily_settlement();
        DB::instance().daily_advance();
    }
    
    summary();
}
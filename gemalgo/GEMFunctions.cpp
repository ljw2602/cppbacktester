//
//  GEMFunctions.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/26/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include "GEMFunctions.hpp"

//#define PRINT 1


void preprocess_data(unsigned yr, unsigned mon)
{
    // Advance all data pointers by yr and mon
    // DB::advance() checks data is longer than requested duration
    
    DB::instance().advance(yr, mon);
    assert(mon < 12);
    std::cout << "Trading starts on " << *DB::instance().daily_dt().begin() << std::endl;
}


double rate_of_change(const std::string& name,
                      unsigned yr, unsigned mon)
{
    // Calculates return over x year and y month using adjusted close price
    
    unsigned lookback_mon = yr*12 + mon;
    
    TA ta;
    PriceSeries series = DB::instance().get(name).monthly();
    PriceSeries::const_iterator pSeries = DB::instance().get(name).pMonthly();
    TA::ROCRes resROC = ta.ROC(series.adjclose(pSeries,lookback_mon+1), lookback_mon);
    
    return resROC.roc[0];
}

std::map<double, std::string, std::greater<double> > rate_of_change_map(const unsigned lookback_yr,
                                                                        const unsigned lookback_mo)
{
    // Calculates one year return using adjusted close price for all symbols in DB
    
    std::map<double, std::string, std::greater<double> > ret;  // map with descending order
    
    std::vector<std::string> symbols = DB::instance().dblist();
    for (std::vector<std::string>::const_iterator it = symbols.begin(); it != symbols.end(); it++ )
        ret.insert( std::pair<double, std::string>(rate_of_change(*it, lookback_yr, lookback_mo), *it) );
    
    return ret;
}

std::map<double, std::string, std::greater<double> > monthly_estimation(const boost::gregorian::date& today,
                                                                        const unsigned lookback_yr,
                                                                        const unsigned lookback_mo)
{
    // Return estimation result
    
    std::map<double, std::string, std::greater<double> > est = rate_of_change_map(lookback_yr, lookback_mo);
#ifdef PRINT
    std::cout << "Return (%) over " << lookback_yr << " year " << lookback_mo << " month on " << today << std::endl;
    for (std::map<double, std::string>::const_iterator it = est.begin(); it != est.end(); it++) {
        std::cout << it->second << "\t" << it->first << std::endl;
    }
#endif
    return est;
}


std::map<std::string, int> monthly_allocation(const std::map<double, std::string, std::greater<double> >& est,
                                              double minimum,
                                              const boost::gregorian::date& today)
{
    // Allocate a fixed percentage of total capital to one winner (long only)
    // No additional buy/sell executions until the winner is switched
    
    static std::string old_winner;  // this static variables save old winner and target
    static std::map<std::string, int> old_target;
    double invest_ratio = 0.8;
    
    std::map<double, std::string>::const_iterator it = est.begin();  // est is sorted in decreasing order
    const double winner_ret(it->first);
    const std::string this_winner(it->second);
    
    std::map<std::string, int> this_target;
    std::vector<std::string> names = DB::instance().dblist();
    
    for (int i = 0; i < names.size(); i++)
        this_target.insert( std::pair<std::string, int>(names[i], 0) );  // all to zero by default
    
    if (winner_ret < minimum)
    {
        // If highest return is negative, close all positions
#ifdef PRINT
        std::cout << winner_ret << " (highest ret) < " << minimum << " (treasury ret): close all positions on " << today << std::endl;
#endif
    }
    else if (!old_winner.empty() && old_winner == this_winner)
    {
        // When the winner hasn't changed, keep the same target shares
        this_target = old_target;
#ifdef PRINT
       std::cout << "Keep invest in " << this_winner << " on " << today << std::endl;
#endif
    }
    else if (old_winner.empty() || (!old_winner.empty() && old_winner != this_winner))
    {
        // When there is a new winner, allocate 80% of latest total balance to the new winner
        double total = BalanceSet::instance().latest_total();
        double price = DB::instance().get(this_winner).pMonthly()->second->close();
        
        int target_share = (0.8*total) / price;
        this_target.at(this_winner) = target_share;
#ifdef PRINT
        std::cout << "Invest in " << this_winner << " 80% of capital; 0.8*" << total << "/" << price << " = " <<target_share << " shares on " << today << std::endl;
#endif
        
        old_winner = this_winner;
        old_target = this_target;

    } else {
        throw std::exception();  // NEED TO BE MORE SPECIFIED
    }
    
    return this_target;
}

void daily_settlement(std::map<std::string, int>& share_book,
                      const double netcash,
                      const boost::gregorian::date& today)
{
    // EOD split & dividend recognition
    // EOD balance check
    
    double netdividend = BalanceSet::instance().split_and_dividend(share_book, today);
    BalanceSet::instance().update_capital(share_book, netcash + netdividend, today);
}


void summary()
{
}
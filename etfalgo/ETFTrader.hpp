//
//  ETFTrader.hpp
//  cppbacktester
//
//  Created by Jeongwon on 4/14/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#ifndef ETFTrader_hpp
#define ETFTrader_hpp

#include <map>
#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>

#include "Trader.hpp"
#include "TreasurySeries.hpp"
#include "DB.hpp"
#include "TA.hpp"
#include "BlackLitterman.hpp"
#include "BalanceSet.hpp"

//1/ FIND A WAY TO INCORPORATE USBOND INTO FRAMEWORK, ALLOCATE 100% WHEN WINNER < RF

class ETFTrader: public Trader
{
public:
    ETFTrader(const double initial): initial_capital(initial) {}  // <-- to be removed in the future
    ETFTrader(const double initial, const TreasurySeries& ts);
    
    void set_bond(const std::string& bond_sym) { BOND = bond_sym; }
    void run() throw(TraderException);
    
private:
    const double initial_capital;
    boost::gregorian::date first_trading_day;
    std::shared_ptr<TreasurySeries> rf;  // annualized treasury yield
    std::string BOND;
    
    /*
     preprocess_data
     date_check
     daily_execution
     monthly_estimation (requires rate_of_change, rate_of_change_map)
     monthly_allocation
     get_rf
     daily_settlement
     */

    void preprocess_data(unsigned yr, unsigned mon);

    void date_check(const boost::gregorian::date& today);
    
    double daily_execution(std::map<std::string, int>& share_book,
                           std::map<std::string, int>& order_book,
                           const boost::gregorian::date& today);
    
    double rate_of_change(const std::string& name, unsigned yr, unsigned mon);
    
    std::map<double, std::string, std::greater<double> > rate_of_change_map(const unsigned lookback_yr, const unsigned lookback_mo);

    std::map<double, std::string, std::greater<double> > monthly_estimation(const boost::gregorian::date& today, const unsigned lookback_yr, const unsigned lookback_mo);
    
    std::map<std::string, int> monthly_allocation(const std::map<double, std::string, std::greater<double> >& est, double minimum, const boost::gregorian::date& today);

    void daily_settlement(std::map<std::string, int>& share_book,
                          const double netcash,
                          const boost::gregorian::date& today);

};

#endif /* ETFTrader_hpp */

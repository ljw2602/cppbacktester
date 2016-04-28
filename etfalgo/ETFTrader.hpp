//
//  ETFTrader.hpp
//  cppbacktester
//
//  Created by Jeongwon on 4/14/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#ifndef ETFTrader_hpp
#define ETFTrader_hpp

#include "Trader.hpp"
#include "TreasurySeries.hpp"
#include "ETFFunctions.hpp"


class ETFTrader: public Trader
{
public:
    ETFTrader(const double initial): initial_capital(initial) {}  // <-- to be removed in the future
    ETFTrader(const double initial, const TreasurySeries& ts);
    
    void run() throw(TraderException);
    
private:
    const double initial_capital;
    boost::gregorian::date first_trading_day;
    std::shared_ptr<TreasurySeries> rf;  // annualized treasury yield
    
    double daily_execution(std::map<std::string, int>& share_book,
                           std::map<std::string, int>& order_book,
                           const boost::gregorian::date& today);
    
    void date_check(const boost::gregorian::date& today);
    
};

#endif /* ETFTrader_hpp */

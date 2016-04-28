//
//  GEMTrader.hpp
//  cppbacktester
//
//  Created by Jeongwon on 4/14/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#ifndef GEMTrader_hpp
#define GEMTrader_hpp

#include "Trader.hpp"
#include "TreasurySeries.hpp"
#include "GEMFunctions.hpp"

//1/ TREASURY DATA IS NOT CORRECT; I READ 1YR AND 2YR NUMBERS ARE READ  v
//2/ TURN OFF BLACKLITTERMAN v
//3/ GEMTRADER ERROR CORRECT v
//4/ TURN ON ETFALGO SOMETIMES TO MAKE SURE EVERYTHING IS COMPATIBLE
//5/ BALANCESET TRACK # SHARES TOO- ADD _SHARE TO BALANCE CLASS, WHEN UPDATE_CAPITAL IS CALLED UPDATE SHARE v
//6/ COMPARE WITH EXCEL
//7/ INCREASE INVESTMENT PORTION; WHY ERROR WHEN RATIO != 0.8

class GEMTrader: public Trader
{
public:
    GEMTrader(const double initial): initial_capital(initial) {}  // <-- to be removed in the future
    GEMTrader(const double initial, const TreasurySeries& ts);
    
    void run() throw(TraderException);
    
private:
    const double initial_capital;
    boost::gregorian::date first_trading_day;
    std::shared_ptr<TreasurySeries> rf;  // annualized treasury yield
    
    double daily_execution(std::map<std::string, int>& share_book,
                           std::map<std::string, int>& order_book,
                           const boost::gregorian::date& today);
    
    void date_check(const boost::gregorian::date& today);
    double get_rf(const boost::gregorian::date& today);
};

#endif /* GEMTrader_hpp */

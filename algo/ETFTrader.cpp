//
//  ETFTrader.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/14/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include "ETFTrader.hpp"

ETFTrader::ETFTrader(const std::string& initial): initial_capital(initial) { }


void ETFTrader::proprocess_data(unsigned yr, unsigned mon) {
    /*
     1. Advance all data pointers by max(look-back-period)
     */
    // Advance daily and monthly iterators to 1 year from beginning
    //    std::for_each(_db.begin(), _db.end(), run_FF_EOD_iterator);
    //    std::for_each(_db.begin(), _db.end(), run_FF_EOM_iterator);
    //    std::for_each(_benchmark.begin(), _benchmark.end(), run_FF_EOD_iterator);
    //    std::for_each(_benchmark.begin(), _benchmark.end(), run_FF_EOM_iterator);
}


void estimation() {
    /*
     1. Project expected winner and loser
     2. Define certainty of the projection
     */
}


void allocation() {
    /*
     1. Allocate asset weight based on the estimation
     2. Use Black-Litterman
     */
}


void daily_execution() {
    /*
     1. Flush all execution orders from prior EOD
     */
}


void daily_settlement() {
    /*
     1. EOD balance check
     2. EOD split & dividend recognition
     */
}


void summary() {
    
}


void ETFTrader::run() throw(TraderException)
{
    proprocess_data(1,0);
    
    // for every day
    for(int day = 0; day < 10; day++ ) {
        daily_execution();
        
        // every EOM
        if (day == 5) {
            estimation();
            allocation();
        }
        
        daily_settlement();
    }
    
    summary();
}
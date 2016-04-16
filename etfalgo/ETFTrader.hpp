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
#include "DB.hpp"

class ETFTrader: public Trader
{
public:
    ETFTrader(const std::string& initial);
    
    void run() throw(TraderException);
    
private:
    const std::string initial_capital;
//    void preprocess_data(unsigned yr, unsigned mon);
    
};

#endif /* ETFTrader_hpp */

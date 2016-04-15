//
//  LongPosition.hpp
//  cppbacktester
//
//  Created by Jeongwon on 4/13/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#ifndef LongPosition_hpp
#define LongPosition_hpp

#include "Position.hpp"

class LongPosition: public Position
{
public:
    LongPosition(ID id, const std::string& symbol, const boost::gregorian::date& dt, const double price, unsigned size) throw(PositionException);
    virtual ~LongPosition(void) { }
    
    virtual Type type(void) const { return LONG; }
    virtual std::string type_str(void) const { return "Long"; }
    
    virtual double avgEntryPrice(void) const throw(PositionException) { return _avgBuyPrice; }
    virtual double avgExitPrice(void) const throw(PositionException) { return _avgSellPrice; }

    virtual void buy(const boost::gregorian::date& dt, const double price, unsigned size) throw(PositionException);
    virtual void sell(const boost::gregorian::date& dt, const double price, unsigned size) throw(PositionException);
    virtual void sell_short(const boost::gregorian::date& dt, const double price, unsigned size) throw(PositionException);
    virtual void cover(const boost::gregorian::date& dt, const double price, unsigned size) throw(PositionException);
    virtual void close(const boost::gregorian::date& dt, const double price) throw(PositionException);
    
    //! The number of buy/sell executions on this position.
    unsigned buys(void) const { return _buys; }
    unsigned sells(void) const { return _sells; }
    
private:
    double _avgBuyPrice;
    double _avgSellPrice;

    unsigned _buys;
    unsigned _sells;

};

#endif /* LongPosition_hpp */

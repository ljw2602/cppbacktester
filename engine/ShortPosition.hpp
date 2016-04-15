//
//  ShortPosition.hpp
//  cppbacktester
//
//  Created by Jeongwon on 4/14/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#ifndef ShortPosition_hpp
#define ShortPosition_hpp

#include "Position.hpp"

class ShortPosition: public Position
{
public:
    ShortPosition(ID id, const std::string& symbol, const boost::gregorian::date& dt, const double price, unsigned size) throw(PositionException);
    virtual ~ShortPosition(void) { }
    
    virtual Type type(void) const { return SHORT; }
    virtual std::string type_str(void) const { return "Short"; }
    
    virtual double avgEntryPrice(void) const throw(PositionException) { return _avgShortPrice; }
    virtual double avgExitPrice(void) const throw(PositionException) { return _avgCoverPrice; }
    
    virtual void buy(const boost::gregorian::date& dt, const double price, unsigned size) throw(PositionException);
    virtual void sell(const boost::gregorian::date& dt, const double price, unsigned size) throw(PositionException);
    virtual void sell_short(const boost::gregorian::date& dt, const double price, unsigned size) throw(PositionException);
    virtual void cover(const boost::gregorian::date& dt, const double price, unsigned size) throw(PositionException);
    virtual void close(const boost::gregorian::date& dt, const double price) throw(PositionException);
    
    //! The number of buy/sell executions on this position.
    unsigned shorts(void) const { return _shorts; }
    unsigned covers(void) const { return _covers; }
    
private:
    double _avgShortPrice;
    double _avgCoverPrice;
    
    unsigned _shorts;
    unsigned _covers;
    
};

#endif /* ShortPosition_hpp */

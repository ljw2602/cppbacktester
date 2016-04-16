//
//  Trader.hpp
//  cppbacktester
//
//  Created by Jeongwon on 4/14/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#ifndef Trader_hpp
#define Trader_hpp

#include "Position.hpp"
#include "LongPosition.hpp"
#include "ShortPosition.hpp"
#include "PositionSet.hpp"
#include <boost/date_time/gregorian/gregorian.hpp>

class TraderException: public std::exception
{
public:
    TraderException(const std::string& msg):
    _Str("TraderException: ")
    {
        _Str += msg;
    }
    
    virtual ~TraderException(void) throw() { }
    virtual const char *what(void) const throw() { return _Str.c_str(); }
    
protected:
    std::string _Str;
};


/*!
 Simulates Buy, Sell, Sell Short and Cover transactions. All open and closed Position objects are grouped in a local PositionSet collection.
 Transactions can create new Position objects or modify existing ones by calling the appropriate method using the unique Position::ID identifier.
 \see positions()
 */
class Trader
{
public:
    Trader(void);
    virtual ~Trader(void) { }
    
    // Buy and sell
    Position::ID buy(const std::string& symbol, const boost::gregorian::date& dt, const double price, unsigned size = 1) throw(TraderException);
    void buy(Position::ID id, const boost::gregorian::date& dt, const double price, unsigned size = 1) throw(TraderException);
    void sell(Position::ID id, const boost::gregorian::date& dt, const double price, unsigned size = 1) throw(TraderException);
    
    // Short and cover
    Position::ID sell_short(const std::string& symbol, const boost::gregorian::date& dt, const double price, unsigned size = 1) throw(TraderException);
    void sell_short(Position::ID, const boost::gregorian::date& dt, const double price, unsigned size = 1) throw(TraderException);
    void cover(Position::ID, const boost::gregorian::date& dt, const double price, unsigned size = 1) throw(TraderException);
    
    // Close position
    void close(Position::ID id, const boost::gregorian::date& dt, const double price) throw(TraderException);
    
    virtual void run() = 0;
    void print(Position::ID id);
    
protected:
    PositionSet positions(void) const { return _Positions; }
    PositionSet positions(const std::string& symbol);
    
//    static Position::ID id(void) { return _pid; }
    
    PositionPtr get(Position::ID id) const throw(TraderException);
    
    static Position::ID _pid; //! Unique Position id
    PositionSet _Positions; //! Complete set of open/closed PositionPtr for this Trader
    
};

#endif /* Trader_hpp */

//
//  LongPosition.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/13/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include "LongPosition.hpp"

LongPosition::LongPosition(ID id,
                           const std::string& symbol,
                           const boost::gregorian::date& dt,
                           const double price,
                           unsigned size) throw(PositionException):
Position(id, symbol),
_buys(0),
_sells(0),
_avgBuyPrice(0),
_avgSellPrice(0)
{
    if( size == 0 )
        throw PositionException("Invalid size");
    
    if( dt.is_not_a_date() )
        throw PositionException("Invalid date");
    
    if( _sExecutions.buy(_symbol, dt, price, size) == Execution::NullID )
        throw PositionException("Invalid buy execution");
    
    _avgBuyPrice = ((_avgBuyPrice * _buys) + (price * size)) / (double)(_buys + size);
    
    _size += size;
    _buys += size;
}


void LongPosition::buy(const boost::gregorian::date& dt, const double price, unsigned size) throw(PositionException)
{
    if( closed() )
        throw PositionException("Position is closed");
    
    if( size == 0 )
        throw PositionException("Invalid size");
    
    if( dt.is_not_a_date() )
        throw PositionException("Invalid date");
    
    if( _sExecutions.buy(_symbol, dt, price, size) == Execution::NullID )
        throw PositionException("Invalid buy execution");
    
    _avgBuyPrice = ((_avgBuyPrice * _buys) + (price * size)) / (double)(_buys + size);
    
    _size += size;
    _buys += size;
}


void LongPosition::sell(const boost::gregorian::date& dt, const double price, unsigned size) throw(PositionException)
{
    if( closed() )
        throw PositionException("Position is closed");
    
    if( size == 0 || size > _size )
        throw PositionException("Invalid size");
    
    if( dt.is_not_a_date() )
        throw PositionException("Invalid date");
    
    if( _sExecutions.sell(_symbol, dt, price, size) == Execution::NullID )
        throw PositionException("Invalid execution");
    
    _avgSellPrice = ((_avgSellPrice * _sells) + (price * size)) / (_sells + size);
    
    _size -= size;
    _sells += size;
}


void LongPosition::sell_short(const boost::gregorian::date& dt, const double price, unsigned size) throw(PositionException)
{
    throw PositionException("Can't sell short long position");
}


void LongPosition::cover(const boost::gregorian::date& dt, const double price, unsigned size) throw(PositionException)
{
    throw PositionException("Can't cover long position");
}


void LongPosition::close(const boost::gregorian::date& dt, const double price) throw(PositionException)
{
    sell(dt, price, _size);
}


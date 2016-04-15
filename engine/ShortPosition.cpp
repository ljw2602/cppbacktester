//
//  ShortPosition.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/14/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include "ShortPosition.hpp"

ShortPosition::ShortPosition(ID id,
                             const std::string& symbol,
                             const boost::gregorian::date& dt,
                             const double price,
                             unsigned size) throw(PositionException):
Position(id, symbol),
_shorts(0),
_covers(0),
_avgShortPrice(0),
_avgCoverPrice(0)
{
    if( size == 0 )
        throw PositionException("Invalid size");
    
    if( dt.is_not_a_date() )
        throw PositionException("Invalid date");
    
    if( _sExecutions.sell_short(_symbol, dt, price, size) == Execution::NullID )
        throw PositionException("Invalid execution");
    
    _avgShortPrice = ((_avgShortPrice * _shorts) + (price * size)) / (double)(_shorts + size);
    
    _size += size;
    _shorts += size;
}


void ShortPosition::sell_short(const boost::gregorian::date& dt, const double price, unsigned size) throw(PositionException)
{
    if( closed() )
        throw PositionException("Position is closed");
    
    if( size == 0 )
        throw PositionException("Invalid size");
    
    if( dt.is_not_a_date() )
        throw PositionException("Invalid date");
    
    if( _sExecutions.sell_short(_symbol, dt, price, size) == Execution::NullID )
        throw PositionException("Invalid execution");
    
    _avgShortPrice = ((_avgShortPrice * _shorts) + (price * size)) / (double)(_shorts + size);
    
    _size += size;
    _shorts += size;
}


void ShortPosition::cover(const boost::gregorian::date& dt, const double price, unsigned size) throw(PositionException)
{
    if( closed() )
        throw PositionException("Position is closed");
    
    if( size == 0 || size > _size )
        throw PositionException("Invalid size");
    
    if( dt.is_not_a_date() )
        throw PositionException("Invalid date");
    
    if( _sExecutions.cover(_symbol, dt, price, size) == Execution::NullID )
        throw PositionException("Invalid execution");
    
    _avgCoverPrice = ((_avgCoverPrice * _covers) + (price * size)) / (_covers + size);
    
    _size -= size;
    _covers += size;
}


void ShortPosition::close(const boost::gregorian::date& dt, const double price) throw(PositionException)
{
    cover(dt, price, _size);
}


void ShortPosition::buy(const boost::gregorian::date& dt, const double price, unsigned size) throw(PositionException)
{
    throw PositionException("Can't buy short position");
}


void ShortPosition::sell(const boost::gregorian::date& dt, const double price, unsigned size) throw(PositionException)
{
    throw PositionException("Short position");
}

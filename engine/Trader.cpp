//
//  Trader.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/14/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include "Trader.hpp"
Position::ID Trader::_pid = 0;


Trader::Trader(void)
{
}


// Buy a new position
double Trader::buy(const std::string& symbol, const boost::gregorian::date& dt, const double price, unsigned size) throw(TraderException)
{
    // Create new position
    PositionPtr pPos;
    
    // Buy position
    try {
        pPos = PositionPtr(new LongPosition(++_pid, symbol, dt, price, size));
    } catch( const std::exception& ex ) {
        throw TraderException(ex.what());
    }
    
    // Add new position to trader collection
    if( _Positions.insert(pPos).first == _Positions.end() )
        throw TraderException("Can't add new long position");
    
    // Return decrease in cash from the purchase
    return -price*size;
}


// Add buy execution to an existing position
double Trader::buy(Position::ID id, const boost::gregorian::date& dt, const double price, unsigned size) throw(TraderException)
{
    // Find existing position
    PositionSet::const_iterator iter = _Positions.find(id, pos_comp_id());
    if( iter == _Positions.end() )
        throw TraderException("Can't find position");
    
    // Add buy transaction to existing position
    PositionPtr pPos = *iter;
    try {
        pPos->buy(dt, price, size);
    } catch( const std::exception& ex ) {
        throw TraderException(ex.what());
    }
    
    // Return decrease in cash from the purchase
    return -price*size;
}


// Sell an existing long position
double Trader::sell(Position::ID id, const boost::gregorian::date& dt, const double price, unsigned size) throw(TraderException)
{
    // Find existing position
    PositionSet::const_iterator iter = _Positions.find(id, pos_comp_id());
    if( iter == _Positions.end() )
        throw TraderException("Can't find position");
    
    // Add sell transaction to existing position
    PositionPtr pPos = *iter;
    try {
        pPos->sell(dt, price, size);
    } catch( const std::exception& ex ) {
        throw TraderException(ex.what());
    }
    
    // Return increase in cash from the sell
    return price*size;
}


double Trader::sell_short(const std::string& symbol, const boost::gregorian::date& dt, const double price, unsigned size) throw(TraderException)
{
    PositionPtr pPos;
    
    try {
        pPos = PositionPtr(new ShortPosition(++_pid, symbol, dt, price, size));
    } catch( const std::exception& ex ) {
        throw TraderException(ex.what());
    }
    
    if( _Positions.insert(pPos).first == _Positions.end() )
        throw TraderException("Can't add short position");
    
    // Return increase in cash from the sell
    return price*size;
}


double Trader::sell_short(Position::ID id, const boost::gregorian::date& dt, const double price, unsigned size) throw(TraderException)
{
    // Find existing position
    PositionSet::const_iterator iter = _Positions.find(id, pos_comp_id());
    if( iter == _Positions.end() )
        throw TraderException("Can't find position");
    
    // Add short execution to existing position
    PositionPtr pPos = *iter;
    try {
        pPos->sell_short(dt, price, size);
    } catch( const std::exception& ex ) {
        throw TraderException(ex.what());
    }
    
    // Return increase in cash from the sell
    return price*size;
}


double Trader::cover(Position::ID id, const boost::gregorian::date& dt, const double price, unsigned size) throw(TraderException)
{
    // Find existing position
    PositionSet::const_iterator iter = _Positions.find(id, pos_comp_id());
    if( iter == _Positions.end() )
        throw TraderException("Can't find position");
    
    // Add cover execution to existing position
    PositionPtr pPos = *iter;
    try {
        pPos->cover(dt, price, size);
    } catch( const std::exception& ex ) {
        throw TraderException(ex.what());
    }
    
    // Return decrease in cash from the purchase
    return -price*size;
}


double Trader::close(Position::ID id, const boost::gregorian::date& dt, const double price) throw(TraderException)
{
    // Find existing position
    PositionSet::const_iterator iter = _Positions.find(id, pos_comp_id());
    if( iter == _Positions.end() )
        throw TraderException("Can't find position");
    
    // Close position
    PositionPtr pPos = *iter;
    try {
        pPos->close(dt, price);
    } catch( const std::exception& ex ) {
        throw TraderException(ex.what());
    }
    
    // Return change in cash from the closure
    if (pPos->type() == Position::LONG) {
        return price * pPos->size();
    } else if (pPos->type() == Position::SHORT) {
        return -price * pPos->size();
    } else {
        throw TraderException("Unknown position type");
    }
}


PositionSet Trader::positions( const std::string& symbol )
{
    PositionSet psSymbol;
    
    for( PositionSet::iterator iter = _Positions.begin(); iter != _Positions.end(); ++iter )
        if( symbol == (*iter)->symbol() )
            psSymbol.insert(*iter);
    
    return psSymbol;
}


PositionPtr Trader::get( Position::ID id ) const throw(TraderException)
{
    PositionSet::const_iterator citer = _Positions.find(id, pos_comp_id());
    if( citer == _Positions.end() ) {
        std::stringstream ss;
        ss << "Can't find position id " << id;
        throw TraderException(ss.str());
    }
    
    return *citer;
}


void Trader::print(Position::ID id)
{
    // Find existing position
    PositionSet::const_iterator iter = _Positions.find(id, pos_comp_id());
    if( iter == _Positions.end() )
        throw TraderException("Can't find position");
    
    (*iter)->print();
}
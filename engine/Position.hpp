//
//  Position.hpp
//  cppbacktester
//
//  Created by Jeongwon on 4/13/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#ifndef Position_hpp
#define Position_hpp

#include "ExecutionSet.hpp"


class PositionException: public std::exception
{
public:
    PositionException(const std::string& msg):
    _Str("PositionException: ")
    {
        _Str += msg;
    }
    
    virtual ~PositionException(void) throw() { }
    virtual const char *what() const throw() { return _Str.c_str(); }
    
protected:
    std::string _Str;
};


//class Position;
//typedef boost::shared_ptr<Position> PositionPtr;

class Position
{
public:
    typedef unsigned long ID;
    
    enum Type {
        LONG,
        SHORT
    };
    
public:
    virtual ~Position(void) { }
    
    //! Default comparison by Position ID.
    bool operator<(const Position& p) const { return _id < p._id; }
    bool operator==(const Position& p) const { return _id == p._id; }
    
    ID id(void) const { return _id; }
    std::string symbol(void) const { return _symbol; }
    int size(void) const { return _size; }
    
    bool open(void) const { return _size != 0; }
    bool closed(void) const { return !_sExecutions.empty() && !open(); }
    virtual void print(void) const;
    //! Return Position holding period, from first opening execution to last closing position execution,
    //! or the last date in the database for the position symbol.
//    boost::gregorian::date_period hold_period(void) const throw(PositionException);
    
    //! Return entire Position ExecutionSet, or by time
    virtual ExecutionSet executions(void) const { return _sExecutions; }
    virtual const ExecutionPtr first_exec(void) const { return _sExecutions.first_by_date(); }
    virtual const ExecutionPtr last_exec(void) const { return _sExecutions.last_by_date(); }
    
    //! Return Position type.
    virtual Type type(void) const = 0;
    virtual std::string type_str(void) const = 0;
    
    virtual double avgEntryPrice(void) const throw(PositionException) = 0;
    virtual double avgExitPrice(void) const throw(PositionException) = 0;
    
    //! Add an existing Position to this Position. Only valid for StrategyPosition derived class.
    //! \return True if the Position was successfully added, false otherwise.
//    virtual bool add(const PositionPtr pPos) throw(PositionException) = 0;
    
    //! Add Executions at specific price.
    virtual void buy(const boost::gregorian::date& dt, const double price, unsigned size) throw(PositionException) = 0;
    virtual void sell(const boost::gregorian::date& dt, const double price, unsigned size) throw(PositionException) = 0;
    virtual void sell_short(const boost::gregorian::date& dt, const double price, unsigned size) throw(PositionException) = 0;
    virtual void cover(const boost::gregorian::date& dt, const double price, unsigned size) throw(PositionException) = 0;
    virtual void close(const boost::gregorian::date& dt, const double price) throw(PositionException) = 0;
    
protected:
    Position(ID id, const std::string& symbol);
    
    const ID _id;
    const std::string _symbol;
    unsigned _size;
    
    ExecutionSet _sExecutions;
};

#endif /* Position_hpp */

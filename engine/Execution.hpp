//
//  Execution.hpp
//  cppbacktester
//
//  Created by Jeongwon on 4/13/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#ifndef Execution_hpp
#define Execution_hpp

#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>


class Execution
{
public:
    enum Side {
        BUY,
        SELL,
        SHORT,
        COVER
    };
    
    typedef unsigned long ID;
    static const ID NullID;
    
    virtual ~Execution(void) { }
    
    //! Executions will be ordered by their ID.
    bool operator<(const Execution& e) const { return _id < e._id; }
    
    std::string symbol(void) const { return _symbol; }
    ID id(void) const { return _id; }
    boost::gregorian::date dt(void) const { return _dt; }
    double price(void) const { return _price; }
    unsigned size(void) const { return _size; }
    virtual std::string action(void) const = 0;
    virtual Side side(void) const = 0;
    
    //! Print all execution data.
    void print(void) const;
    
    
protected:
    Execution(const std::string& symbol,
              ID id,
              const boost::gregorian::date& dt,
              const double price,
              unsigned size);
    
    const std::string _symbol;
    const ID _id;
    const boost::gregorian::date _dt;
    const double _price;
    const unsigned _size;
};


class BuyExecution: public Execution
{
public:
    BuyExecution(const std::string& symbol,
                 Execution::ID id,
                 const boost::gregorian::date& dt,
                 const double price,
                 unsigned size);
    
    std::string action(void) const { return "Bought "; }
    Side side(void) const { return BUY; }
};


class SellExecution: public Execution
{
public:
    SellExecution(const std::string& symbol,
                  Execution::ID id,
                  const boost::gregorian::date& dt,
                  const double price,
                  unsigned size);
    
    std::string action(void) const { return "Sold   "; }
    Side side(void) const { return SELL; }
};


class SellShortExecution: public Execution
{
public:
    SellShortExecution(const std::string& symbol,
                       Execution::ID id,
                       const boost::gregorian::date& dt,
                       const double price,
                       unsigned size);
    
    std::string action(void) const { return "Shorted"; }
    Side side(void) const { return SHORT; }
};


class CoverExecution: public Execution
{
public:
    CoverExecution(const std::string& symbol,
                   Execution::ID id,
                   const boost::gregorian::date& dt,
                   const double price,
                   unsigned size);
    
    std::string action(void) const { return "Covered"; }
    Side side(void) const { return COVER; }
};

#endif /* Execution_hpp */

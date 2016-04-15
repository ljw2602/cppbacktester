//
//  Execution.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/13/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include "Execution.hpp"
#include <iostream>

const Execution::ID Execution::NullID = 0;


Execution::Execution(const std::string& symbol,
                     Execution::ID id,
                     const boost::gregorian::date& dt,
                     const double price,
                     unsigned size): _symbol(symbol), _id(id), _dt(dt), _price(price), _size(size) {}


void Execution::print(void) const
{
    std::cout << action()
    << ' ' << _size
    << " @" << _price
    << ' ' << to_simple_string(_dt)
    << " (" << _dt.day_of_week() << ")"
    << " [Exe ID: " << id() << "]";
}


BuyExecution::BuyExecution(const std::string& symbol,
                           Execution::ID id,
                           const boost::gregorian::date& dt,
                           const double price,
                           unsigned size): Execution(symbol, id, dt, price, size)
{
    std::cout << symbol << " ";
    this->print();
    std::cout << std::endl;
    //    BalanceSet::instance().update_capital(this);
}


SellExecution::SellExecution(const std::string& symbol,
                             Execution::ID id,
                             const boost::gregorian::date& dt,
                             const double price,
                             unsigned size): Execution(symbol, id, dt, price, size)
{
    std::cout << symbol << " ";
    this->print();
    std::cout << std::endl;
    //    BalanceSet::instance().update_capital(this);
}


SellShortExecution::SellShortExecution(const std::string& symbol,
                                       Execution::ID id,
                                       const boost::gregorian::date& dt,
                                       const double price,
                                       unsigned size): Execution(symbol, id, dt, price, size)
{
    
    std::cout << symbol << " ";
    this->print();
    std::cout << std::endl;
    //    BalanceSet::instance().update_capital(this);
}


CoverExecution::CoverExecution(const std::string& symbol,
                               Execution::ID id,
                               const boost::gregorian::date& dt,
                               const double price,
                               unsigned size): Execution(symbol, id, dt, price, size)
{
    
    std::cout << symbol << " ";
    this->print();
    std::cout << std::endl;
    //    BalanceSet::instance().update_capital(this);
}
//
//  ExecutionSet.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/13/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include "ExecutionSet.hpp"


Execution::ID ExecutionSet::_eid = 0;

ExecutionSet::ExecutionSet(void)
{
}

void ExecutionSet::print(void) const
{
    for( by_date::iterator iter = get<date_key>().begin(); iter != get<date_key>().end(); ++iter ) {
        //std::cout << " ";
        (*iter)->print();
        std::cout << std::endl;
    }
}

Execution::ID ExecutionSet::buy(const std::string& symbol, const boost::gregorian::date& dt, const double price, unsigned size)
{
    ExecutionPtr pExe(new BuyExecution(symbol, ++_eid, dt, price, size));
    if( insert(pExe).second == false )
        return Execution::NullID;
    
    return _eid;
}


Execution::ID ExecutionSet::sell(const std::string& symbol, const boost::gregorian::date& dt, const double price, unsigned size)
{
    ExecutionPtr pExe(new SellExecution(symbol, ++_eid, dt, price, size));
    if( insert(pExe).second == false )
        return Execution::NullID;
    
    return _eid;
}


Execution::ID ExecutionSet::sell_short(const std::string& symbol, const boost::gregorian::date& dt, const double price, unsigned size)
{
    ExecutionPtr pExe(new SellShortExecution(symbol, ++_eid, dt, price, size));
    if( insert(pExe).second == false )
        return Execution::NullID;
    
    return _eid;
}


Execution::ID ExecutionSet::cover(const std::string& symbol, const boost::gregorian::date& dt, const double price, unsigned size)
{
    ExecutionPtr pExe(new CoverExecution(symbol, ++_eid, dt, price, size));
    if( insert(pExe).second == false )
        return Execution::NullID;
    
    return _eid;
}
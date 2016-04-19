//
//  Balance.hpp
//  cppbacktester
//
//  Created by Jeongwon on 4/19/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#ifndef Balance_hpp
#define Balance_hpp

#include <iostream>
#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>


class BalanceException: public std::exception
{
public:
    BalanceException(const std::string& msg):
    _Str("BalanceException: ")
    {
        _Str += msg;
    }
    
    virtual ~BalanceException(void) throw() { }
    virtual const char *what(void) const throw() { return _Str.c_str(); }
    
protected:
    std::string _Str;
};


class Balance {
public:
    Balance(std::string& name_,
            boost::gregorian::date dt_,
            double balance_): _name(name_), _dt(dt_), _balance(balance_) {}
    Balance(const std::string& name_,
            const boost::gregorian::date dt_,
            const double balance_): _name(name_), _dt(dt_), _balance(balance_) {}
    
    std::string name() const { return _name; }
    boost::gregorian::date dt() const { return _dt; }
    double balance() const { return _balance; }
    
    void print() const {
        std::cout << _name << "\t" << _dt << "\t" << _balance << std::endl;
    }
    
private:
    boost::gregorian::date _dt;
    std::string _name;
    double _balance;
    
};

#endif /* Balance_hpp */

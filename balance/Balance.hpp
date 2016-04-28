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
            double balance_,
            int shares_ = 0): _name(name_), _dt(dt_), _balance(balance_), _shares(shares_) {}
    Balance(const std::string& name_,
            const boost::gregorian::date dt_,
            const double balance_,
            const int shares_ = 0): _name(name_), _dt(dt_), _balance(balance_), _shares(shares_) {}
    
    boost::gregorian::date dt() const { return _dt; }
    std::string name() const { return _name; }
    double balance() const { return _balance; }
    int shares() const { return _shares; }
    
    void print() const {
        std::cout << _name << "\t" << _dt << "\t" << _balance << "\t" << _shares << std::endl;
    }
    
private:
    boost::gregorian::date _dt;
    std::string _name;
    double _balance;
    int _shares;
    
};

#endif /* Balance_hpp */

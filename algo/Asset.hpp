//
//  Asset.hpp
//  cppbacktester
//
//  Created by Jeongwon on 4/13/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#ifndef Asset_hpp
#define Asset_hpp

#include "PriceSeries.hpp"
#include "ActionSeries.hpp"


class AssetException: public std::exception
{
public:
    AssetException(const std::string& msg):
    _Str("AssetException: ")
    {
        _Str += msg;
    }
    
    virtual ~AssetException(void) throw() { }
    virtual const char *what() const throw() { return _Str.c_str(); }
    
private:
    std::string _Str;
};


class Asset{
public:
    Asset(const std::string& price_file, const std::string& action_file, FileDriver& driver, const boost::gregorian::date& begin, const boost::gregorian::date& end);
    
    const PriceSeries& daily() const { return *_daily; }
    const PriceSeries& monthly() const { return *_monthly; }
    const ActionSeries& action() const { return *_action; }
    
    PriceSeries::const_iterator pDaily() const { return _pDaily; }
    PriceSeries::const_iterator pMonthly() const { return _pMonthly; }
    ActionSeries::const_iterator pAction() const { return _pAction; }
    
private:
    std::shared_ptr<PriceSeries> _daily, _monthly;
    std::shared_ptr<ActionSeries> _action;
    
    mutable PriceSeries::const_iterator _pDaily, _pMonthly;
    mutable ActionSeries::const_iterator _pAction;
};

#endif /* Asset_hpp */

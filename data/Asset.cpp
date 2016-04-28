//
//  Asset.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/13/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include "Asset.hpp"

Asset::Asset(const std::string& price_file, FileDriver& driver, const boost::gregorian::date& begin, const boost::gregorian::date& end)
{
    _daily = std::shared_ptr<PriceSeries>(new PriceSeries(driver, price_file, begin, end));
    _monthly = _daily->monthly();
//    _action = std::shared_ptr<ActionSeries>(new ActionSeries(driver, action_file, begin, end));
    
    // pointers are assigned after advancing to the EOM of the first month
    _pDaily = _daily->last_in_month(begin.year(), begin.month());
    if (_pDaily == _daily->end())
        throw AssetException("Advance daily price data to EOM failed");
    
    _pMonthly = _monthly->begin();
    
}


Asset::Asset(const std::string& price_file, const std::string& action_file, FileDriver& driver, const boost::gregorian::date& begin, const boost::gregorian::date& end)
{
    _daily = std::shared_ptr<PriceSeries>(new PriceSeries(driver, price_file, begin, end));
    _monthly = _daily->monthly();
    _action = std::shared_ptr<ActionSeries>(new ActionSeries(driver, action_file, begin, end));
    
    // pointers are assigned after advancing to the EOM of the first month
    _pDaily = _daily->last_in_month(begin.year(), begin.month());
    if (_pDaily == _daily->end())
        throw AssetException("Advance daily price data to EOM failed");
    
    _pMonthly = _monthly->begin();
    
    _pAction = _action->after(_pMonthly->first, 0);  // advance pAction to the one after the first trading day

    if (_pAction == _action->end())
        std::cout << "No action is recorded during this period" << std::endl;
}


void Asset::advance(unsigned yr, unsigned mon)
{
    unsigned new_mon = _pDaily->first.month() + mon;
    unsigned new_yr = _pDaily->first.year() + yr;
    
    unsigned new_mon_rolled = (new_mon-1)%12 + 1;  // 11->11, 12->12, 13->1, 14->2, ...
    unsigned new_yr_rolled = new_yr + (new_mon-1)/12;

    _pDaily = _daily->last_in_month(new_yr_rolled, new_mon_rolled);
    _pMonthly = _monthly->last_in_month(new_yr_rolled, new_mon_rolled);
    
//    _pDaily = _daily->last_in_month(_pDaily->first.year()+yr, _pDaily->first.month()+mon);
//    _pMonthly = _monthly->last_in_month(_pMonthly->first.year()+yr, _pMonthly->first.month()+mon);
    
    if (_action) {
        _pAction = _action->after(_pMonthly->first, 0);  // advance pAction to the one after the first trading day
    }
}

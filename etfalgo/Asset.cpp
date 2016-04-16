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
    
//    _pAction = _action->last_in_month(begin.year(), begin.month()+1);
//    if (_pAction == _action->end())
//        std::cout << "No action is recorded during this period" << std::endl;
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
    
    _pAction = _action->last_in_month(begin.year(), begin.month()+1);
    if (_pAction == _action->end())
        std::cout << "No action is recorded during this period" << std::endl;
}


void Asset::advance(unsigned yr, unsigned mon)
{
    _pDaily = _daily->last_in_month(_pDaily->first.year()+yr, _pDaily->first.month()+mon);
    _pMonthly = _monthly->last_in_month(_pMonthly->first.year()+yr, _pMonthly->first.month()+mon);
    _pAction = _action->last_in_month(_pAction->first.year()+yr, _pAction->first.month()+mon);
}

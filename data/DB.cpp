//
//  DB.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/10/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include "DB.hpp"
#include <assert.h>
#include <string.h>


std::auto_ptr<DB> DB::_pInstance;

DB& DB::instance(void)
{
    if ( _pInstance.get() == 0 ) {
        _pInstance.reset(new DB);
    }
    return *_pInstance;
}

void DB::load(const std::string& symbol, const std::string& price_file, const std::string& action_file, FileDriver& driver, const boost::gregorian::date& begin, const boost::gregorian::date& end)
{
    // Create an Asset object from the input files
    // Insert its pointer to _db
    
    std::shared_ptr<Asset> pt(new Asset(price_file, action_file, driver, begin, end));
    
    if (_db.size() == 0) {
        std::cout << "Data advanced to the EOM of the first month" << std::endl;
        _len = pt->daily().size();
    } else {
        assert(_len == pt->daily().size());  // all data in DB must have the same length
    }
    
    _db.insert( DBmap::value_type(symbol, pt) );
    _dblist.push_back(symbol);
}

const Asset& DB::get(const std::string& symbol) const
{
    // Retreive Asset with requested symbol name
    
    DBmap::const_iterator it;
    if ( (it = _db.find(symbol)) == _db.end() ) {
        throw DBException("Unknown symbol name");
    }
    return *(it->second);
}

void DB::advance(unsigned yr, unsigned mon)
{
    // Advance all data by yr year and mon month
    
    if (_db.size() == 0)
        throw DBException("No data to advance");
    else
        std::cout << "Data advanced by " << yr << " year and " << mon << " months" << std::endl;
    
    try {
        for (DBmap::const_iterator it = _db.begin(); it != _db.end(); it++) {
            boost::gregorian::date start = (*it->second->pDaily()).first;  // date pointed by pDaily
            boost::gregorian::date end =  it->second->daily().rbegin()->first;  // end of PriceSeries
            unsigned monthdiff = (end.year() - start.year())*12 + (end.month() - start.month());
            
            if (yr*12 + mon >= monthdiff)  // check data is longer than requested advancement
                throw DBException("Data is shorter than requested advancement");
            
            it->second->advance(yr, mon);
        }
    } catch (const std::exception& ex ) {
        throw DBException(ex.what());
    }
}


void DB::daily_advance()
{
    // Advance all daily data by one
    
    if (_db.size() == 0)
        throw DBException("No data to advance");
    
    for (DBmap::const_iterator it = _db.begin(); it != _db.end(); it++) {
        it->second->daily_advance();
    }
}


void DB::monthly_advance()
{
    // Advance all monthly data by one
    
    if (_db.size() == 0)
        throw DBException("No data to advance");
    
    for (DBmap::const_iterator it = _db.begin(); it != _db.end(); it++) {
        it->second->monthly_advance();
    }
}


const std::vector<boost::gregorian::date> DB::daily_dt() const
{
    // Extract date from daily PriceSeries
    
    std::vector<boost::gregorian::date> vdate;
    
    DBmap::const_iterator it = _db.begin();  // use the first element of _db to extract daily date
    PriceSeries::const_iterator pit = it->second->pDaily();

    for (; pit != get(it->first).daily().end(); pit++) {
        vdate.push_back(pit->first);
    }
    
    return vdate;
}


const std::vector<boost::gregorian::date> DB::monthly_dt() const
{
    // Extract date from monthly PriceSeries
    
    std::vector<boost::gregorian::date> vdate;
    
    DBmap::const_iterator it = _db.begin();  // use the first element of _db to extract daily date
    PriceSeries::const_iterator pit = it->second->pMonthly();
    
    for (; pit != get(it->first).monthly().end(); pit++) {
        vdate.push_back(pit->first);
    }
    
    return vdate;
}
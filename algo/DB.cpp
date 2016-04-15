//
//  DB.cpp
//  cppbacktester
//
//  Created by Jeongwon on 4/10/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#include "DB.hpp"


std::auto_ptr<DB> DB::_pInstance;

DB& DB::instance(void) {
    if ( _pInstance.get() == 0 ) {
        _pInstance.reset(new DB);
    }
    return *_pInstance;
}

void DB::load(const std::string& symbol, const std::string& price_file, const std::string& action_file, FileDriver& driver, const boost::gregorian::date& begin, const boost::gregorian::date& end) {
    if (_db.size() == 0)
        std::cout << "Data will be advanced to the EOM of the first month" << std::endl;
    
    std::shared_ptr<Asset> pt(new Asset(price_file, action_file, driver, begin, end));
    _db.insert( DBmap::value_type(symbol, pt) );
    _dblist.push_back(symbol);
}

const Asset& DB::get(const std::string& symbol) {
    DBmap::const_iterator it;
    if ( (it = _db.find(symbol)) == _db.end() ) {
        throw DBException("Unknown symbol name");
    }
    return *(it->second);
}

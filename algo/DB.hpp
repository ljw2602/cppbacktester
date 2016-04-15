//
//  DB.hpp
//  cppbacktester
//
//  Created by Jeongwon on 4/10/16.
//  Copyright © 2016 Jeongwon. All rights reserved.
//

#ifndef DB_hpp
#define DB_hpp

#include <memory>
#include <map>

#include "DataEntry.hpp"
#include "Asset.hpp"


class DBException: public std::exception
{
public:
    DBException(const std::string& msg):
    _Str("DBException: ")
    {
        _Str += msg;
    }
    
    virtual ~DBException(void) throw() { }
    virtual const char *what() const throw() { return _Str.c_str(); }
    
private:
    std::string _Str;
};


class DB{
public:
    static DB& instance(void);
    void load(const std::string& symbol, const std::string& price_file, const std::string& action_file, FileDriver& driver, const boost::gregorian::date& begin, const boost::gregorian::date& end);
    const Asset& get(const std::string& symbol);
    std::vector<std::string> dblist() const { return _dblist; }
    
private:
    DB(){}
    static std::auto_ptr<DB> _pInstance;
    typedef std::map<std::string, std::shared_ptr<Asset> > DBmap;
    DBmap _db;
    std::vector<std::string> _dblist;
};


#endif /* DB_hpp */
